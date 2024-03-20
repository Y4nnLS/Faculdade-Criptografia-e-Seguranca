#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define EP1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SIGMA0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ ((x) >> 3))
#define SIGMA1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ ((x) >> 10))

uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void sha256(const uint8_t *message, size_t len, uint32_t hash[8]) {
    uint32_t w[64];
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t t1, t2;

    hash[0] = 0x6a09e667;
    hash[1] = 0xbb67ae85;
    hash[2] = 0x3c6ef372;
    hash[3] = 0xa54ff53a;
    hash[4] = 0x510e527f;
    hash[5] = 0x9b05688c;
    hash[6] = 0x1f83d9ab;
    hash[7] = 0x5be0cd19;

    size_t new_len = ((((len + 8) / 64) + 1) * 64) - 8;
    uint8_t *msg = malloc(new_len + 64);
    memcpy(msg, message, len);
    msg[len] = 0x80;
    for (size_t i = len + 1; i < new_len; ++i)
        msg[i] = 0;
    uint64_t bits_len = len * 8;
    for (int i = 0; i < 8; ++i)
        msg[new_len + i] = (bits_len >> ((7 - i) * 8)) & 0xff;

    for (int i = 0; i < new_len; i += 64) {
        for (int j = 0; j < 16; ++j) {
            w[j] = (msg[i + j*4 + 0] << 24) |
                   (msg[i + j*4 + 1] << 16) |
                   (msg[i + j*4 + 2] << 8) |
                   (msg[i + j*4 + 3]);
        }
        for (int j = 16; j < 64; ++j) {
            w[j] = w[j-16] + SIGMA0(w[j-15]) + w[j-7] + SIGMA1(w[j-2]);
        }

        a = hash[0];
        b = hash[1];
        c = hash[2];
        d = hash[3];
        e = hash[4];
        f = hash[5];
        g = hash[6];
        h = hash[7];

        for (int j = 0; j < 64; ++j) {
            t1 = h + EP1(e) + CH(e, f, g) + K[j] + w[j];
            t2 = EP0(a) + MAJ(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        hash[0] += a;
        hash[1] += b;
        hash[2] += c;
        hash[3] += d;
        hash[4] += e;
        hash[5] += f;
        hash[6] += g;
        hash[7] += h;
    };

    free(msg);  
}

char *sha256_string(const char *message) {
    uint32_t digest[8];
    sha256((const uint8_t *)message, strlen(message), digest);

    // Convert the digest to a string
    char *result = malloc(65); // SHA-256 produces a 256-bit hash, i.e., 32 bytes or 64 hexadecimal characters + '\0'
    sprintf(result, "%08x%08x%08x%08x%08x%08x%08x%08x", digest[0], digest[1], digest[2], digest[3], digest[4], digest[5], digest[6], digest[7]);

    return result;
}



//Função Base64 Encoder \\\\\\\\\\\\\\\\\\\\\\\\\

char intParaChar(int entrada) {
    if (entrada == 1) {
        return '1';
    }
    else if (entrada == 0) {
        return '0';
    }
    return '\0'; // Retornar caractere nulo se entrada não for 0 ou 1
}

int charParaInt(char entrada) {
    if (entrada == '1') {
        return 1;
    }
    else if (entrada == '0') {
        return 0;
    }
    return -1; // Retornar -1 se entrada não for '0' ou '1'
}

char *decimalParaBinario(int decimal) {
    int binario[8] = {0};
    int i = 7;

    // Caso especial para o valor zero
    if (decimal == 0) {
        binario[i] = 0;
        i--;
    }

    while (decimal > 0) {
        int sobra = decimal % 2;
        binario[i] = sobra;
        i--;
        decimal = decimal / 2;
    }

    // Converte o array de binário para string
    char *binario_f = malloc(9); // 8 bits + 1 para o caractere nulo
    int j = 0;
    for (int x = i + 1; x <= 7; x++) {
        binario_f[j] = intParaChar(binario[x]);
        j++;
    }
    binario_f[j] = '\0'; // Adiciona o caractere nulo ao final da string

    return binario_f;
}

int binarioParaDecimal(const char *casaBinaria) {
    int soma = 0;
    int tamanho = strlen(casaBinaria) - 1;
    for (int x = 0; x < 6; x++) {
        char casaChar = casaBinaria[tamanho];
        int potencia = pow(2, x);
        int casaInt = charParaInt(casaChar);
        soma = soma + (casaInt * potencia);
        tamanho--;
    }
    return soma;
}

char *base64_encode(const char *input) {
    static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int input_len = strlen(input);
    int output_len = ((input_len + 2) / 3) * 4 + 1; // Calculating output length
    char *output = malloc(output_len);
    int bits = 0, bit_count = 0;
    int index = 0;

    for (int i = 0; i < input_len; i++) {
        bits = (bits << 8) + (unsigned char)input[i];
        bit_count += 8;

        while (bit_count >= 6) {
            bit_count -= 6;
            output[index++] = base64_chars[(bits >> bit_count) & 0x3F];
        }
    }

    if (bit_count > 0) {
        bits <<= 6 - bit_count;
        output[index++] = base64_chars[bits & 0x3F];
    }

    while (index % 4 != 0)
        output[index++] = '=';

    output[index] = '\0'; // Null-terminate the string

    return output;
}


//Função BASE64 Decoder

char decodeCaractere(char c) {
    const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    const char *ptr = strchr(base64Chars, c);
    if (ptr != NULL) {
        return ptr - base64Chars; // Retorna a posição do caractere na string
    }
    else {
        return -1; // caractere inválido
    }
}

char *base64_decode(const char *StringCodificada) {
    char *decodedString = malloc(strlen(StringCodificada)); // Estimativa do tamanho máximo
    int bits = 0, buffer = 0;
    int index = 0;

    for (int i = 0; StringCodificada[i] != '\0'; i++) {
        char c = StringCodificada[i];
        if (c == '=') break; // padding encontrado, termina a decodificação

        int val = decodeCaractere(c);
        if (val == -1) continue; // caractere inválido, ignorado

        buffer = (buffer << 6) | val;
        bits += 6;

        while (bits >= 8) {
            bits -= 8;
            char decodedChar = buffer >> bits;
            decodedString[index++] = decodedChar;
        }
    }

    decodedString[index] = '\0'; // Null-terminate the string
    return decodedString;
}

int main() {
    
    int op = 0;
    char msg[100];
    
    do{
        printf("\n\nO que deseja fazer?\n");
        printf("[1] Converter mensagem para base64\n");
        printf("[2] Desconverter mensagem em base64\n");
        printf("[3] Converter mensagem para SHA-256\n");
        printf("[0] Encerrar\n");
        scanf("%d", &op);
        
        switch(op){
            
            case 1:
                printf("Informe a mensagem para converter para Base64: ");
                scanf("%s", &msg);
                
                printf("Mensagem em Base64: %s", base64_encode(msg));
                
                break;
                
            case 2:
                printf("Informe a mensagem para desconverter da Base64: ");
                scanf("%s", &msg);
                
                printf("Mensagem descriptografada da base64: %s", base64_decode(msg));
            
                break;
            
            case 3:
                printf("Informe a mensagem para converter para SHA-256: ");
                scanf("%s", &msg);
                
                printf("Mensagem em SHA-256: %s", sha256_string(msg));
                
                break;
                
            case 0:
                printf("Programa encerrado!");
                break;
        }
        
    }while(op!=0);
}