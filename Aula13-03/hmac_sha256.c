#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_SIZE 32

const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void sha256(const unsigned char *message, uint32_t len, uint8_t *digest) {
    uint32_t i, j;
    uint8_t hash[SHA256_DIGEST_SIZE];
    uint32_t h0, h1, h2, h3, h4, h5, h6, h7;
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t *w, *words;
    uint32_t num_blocks = ((len + 8) / SHA256_BLOCK_SIZE) + 1;
    uint8_t *padded_message = (uint8_t *)malloc(num_blocks * SHA256_BLOCK_SIZE);
    
    for (i = 0; i < len; i++)
        padded_message[i] = message[i];
    padded_message[len] = 0x80;
    for (i = len + 1; i < num_blocks * SHA256_BLOCK_SIZE; i++)
        padded_message[i] = 0;
    len *= 8;
    padded_message[num_blocks * SHA256_BLOCK_SIZE - 1] = len;
    words = (uint32_t *)padded_message;

    for (i = 0; i < num_blocks; i++) {
        w = words + i * 16;
        for (j = 0; j < 16; j++)
            w[j] = (w[j] >> 24) | ((w[j] >> 8) & 0x0000FF00) | ((w[j] << 8) & 0x00FF0000) | (w[j] << 24);
    }

    h0 = 0x6a09e667;
    h1 = 0xbb67ae85;
    h2 = 0x3c6ef372;
    h3 = 0xa54ff53a;
    h4 = 0x510e527f;
    h5 = 0x9b05688c;
    h6 = 0x1f83d9ab;
    h7 = 0x5be0cd19;

    for (i = 0; i < num_blocks; i++) {
        w = words + (i * 16);
        for (j = 16; j < 64; j++) {
            w[j] = w[j - 16] + (w[j - 7] >> 3) + w[j - 15] + (w[j - 2] >> 10);
        }
        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;
        f = h5;
        g = h6;
        h = h7;
        for (j = 0; j < 64; j++) {
            uint32_t temp1 = h + ((e >> 6) | (e << 26)) + ((e & f) ^ (~e & g)) + k[j] + w[j];
            uint32_t temp2 = ((a >> 2) | (a << 30)) + ((a & b) ^ (a & c) ^ (b & c));
            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
        h5 += f;
        h6 += g;
        h7 += h;
    }

    memcpy(digest, &h0, 4);
    memcpy(digest + 4, &h1, 4);
    memcpy(digest + 8, &h2, 4);
    memcpy(digest + 12, &h3, 4);
    memcpy(digest + 16, &h4, 4);
    memcpy(digest + 20, &h5, 4);
    memcpy(digest + 24, &h6, 4);
    memcpy(digest + 28, &h7, 4);
    
    free(padded_message);
}

void hmac_sha256(const unsigned char *message, int message_len, const unsigned char *key, int key_len, unsigned char *digest) {
    unsigned char k[SHA256_BLOCK_SIZE];
    if (key_len > SHA256_BLOCK_SIZE) {
        sha256(key, key_len, k);
        key_len = SHA256_DIGEST_SIZE;
    } else {
        memcpy(k, key, key_len);
    }
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++)
        k[i] ^= 0x36;

    unsigned char inner_digest[SHA256_DIGEST_SIZE];
    sha256(k, SHA256_BLOCK_SIZE, inner_digest);
    
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++)
        k[i] ^= (0x36 ^ 0x5c);

    unsigned char *inner_message = (unsigned char *)malloc(message_len + SHA256_BLOCK_SIZE);
    memcpy(inner_message, inner_digest, SHA256_DIGEST_SIZE);
    memcpy(inner_message + SHA256_DIGEST_SIZE, message, message_len);

    sha256(inner_message, message_len + SHA256_BLOCK_SIZE, digest);
    
    free(inner_message);
}

int main() {
    const unsigned char data[] = "Hello, world!";
    const unsigned char key[] = "secretkey";
    unsigned char digest[SHA256_DIGEST_SIZE];

    hmac_sha256(data, strlen(data), key, strlen(key), digest);

    printf("HMAC SHA256: ");
    for (int i = 0; i < SHA256_DIGEST_SIZE; i++) {
        printf("%02x", digest[i]);
    }
    printf("\n");

    return 0;
}