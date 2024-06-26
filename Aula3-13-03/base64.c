#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Tabela de caracteres Base64
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64_encode(const unsigned char *data, size_t input_length) {
    size_t output_length = 4 * ((input_length + 2) / 3);
    char *encoded_data = malloc(output_length + 1);
    if (encoded_data == NULL) return NULL;

    for (size_t i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        
        encoded_data[j++] = base64_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 0 * 6) & 0x3F];
    }

    // Preenche os bytes extras com '='
    for (size_t i = 0; i < (3 - input_length % 3) % 3; i++) {
        encoded_data[output_length - 1 - i] = '=';
    }

    encoded_data[output_length] = '\0';
    return encoded_data;
}

static int base64_decode_char(char c) {
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 26;
    if (c >= '0' && c <= '9')
        return c - '0' + 52;
    if (c == '+')
        return 62;
    if (c == '/')
        return 63;
    if (c == '=')
        return 0; // Ignora os caracteres de preenchimento
    return -1; // Caracter inválido
}

unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length) {
    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length + 1);
    if (decoded_data == NULL) return NULL;

    for (size_t i = 0, j = 0; i < input_length;) {
        int sextet_a = base64_decode_char(data[i++]);
        int sextet_b = base64_decode_char(data[i++]);
        int sextet_c = base64_decode_char(data[i++]);
        int sextet_d = base64_decode_char(data[i++]);

        uint32_t triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

        decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    decoded_data[*output_length] = '\0';
    return decoded_data;
}

int main() {
    const char *original_string = "a";
    size_t original_length = strlen(original_string);
    size_t encoded_length;
    char *encoded_string = base64_encode((const unsigned char *)original_string, original_length);

    printf("String codificada: %s\n", encoded_string);

    unsigned char *decoded_data = base64_decode(encoded_string, strlen(encoded_string), &encoded_length);
    printf("String decodificada: %s\n", decoded_data);

    free(encoded_string);
    free(decoded_data);

    return 0;
}
