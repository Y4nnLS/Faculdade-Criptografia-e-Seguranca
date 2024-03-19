#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_SIZE 32

void sha256_transform(uint32_t *state, const unsigned char *data);
void sha256(const unsigned char *message, unsigned int len, unsigned char *digest);
void hmac_sha256(const unsigned char *key, unsigned int key_len, const unsigned char *message, unsigned int message_len, unsigned char *digest);

void sha256_transform(uint32_t *state, const unsigned char *data) {
    uint32_t temp1, temp2, a, b, c, d, e, f, g, h, i;
    uint32_t m[64];
    uint32_t w[64];
    for (i = 0; i < 16; ++i) {
        m[i] = ((uint32_t)data[i * 4] << 24) | ((uint32_t)data[i * 4 + 1] << 16) |
               ((uint32_t)data[i * 4 + 2] << 8) | ((uint32_t)data[i * 4 + 3]);
    }
    for (; i < 64; ++i) {
        m[i] = m[i - 16] + (m[i - 15] >> 7 ^ m[i - 15] >> 18 ^ m[i - 15] >> 3) + m[i - 7] +
               (m[i - 2] >> 17 ^ m[i - 2] >> 19 ^ m[i - 2] >> 10);
    }
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    f = state[5];
    g = state[6];
    h = state[7];
    for (i = 0; i < 64; ++i) {
        temp1 = h + (f >> 6 ^ f >> 11 ^ f >> 25) + (f & g ^ ~f & h) + 0x428a2f98 + m[i];
        temp2 = (a >> 2 ^ a >> 13 ^ a >> 22) + (a & b ^ a & c ^ b & c);
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

void sha256(const unsigned char *message, unsigned int len, unsigned char *digest) {
    uint32_t state[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    unsigned int i;
    unsigned int offset = 0;
    unsigned int n = len / 64;
    unsigned char block[SHA256_BLOCK_SIZE];
    for (i = 0; i < n; ++i) {
        memcpy(block, message + offset, SHA256_BLOCK_SIZE);
        sha256_transform(state, block);
        offset += SHA256_BLOCK_SIZE;
    }
    memset(block, 0, SHA256_BLOCK_SIZE);
    memcpy(block, message + offset, len % 64);
    block[len % 64] = 0x80;
    if (len % 64 >= 56) {
        sha256_transform(state, block);
        memset(block, 0, SHA256_BLOCK_SIZE);
    }
    *((uint64_t *)(block + 56)) = len * 8;
    sha256_transform(state, block);
    for (i = 0; i < 8; ++i) {
        digest[i * 4 + 0] = (state[i] >> 24) & 0xFF;
        digest[i * 4 + 1] = (state[i] >> 16) & 0xFF;
        digest[i * 4 + 2] = (state[i] >> 8) & 0xFF;
        digest[i * 4 + 3] = (state[i] >> 0) & 0xFF;
    }
}

void hmac_sha256(const unsigned char *key, unsigned int key_len, const unsigned char *message, unsigned int message_len, unsigned char *digest) {
    unsigned char k_ipad[SHA256_BLOCK_SIZE];
    unsigned char k_opad[SHA256_BLOCK_SIZE];
    unsigned char tk[SHA256_DIGEST_SIZE];
    unsigned int i;
    if (key_len > SHA256_BLOCK_SIZE) {
        sha256(key, key_len, tk);
        key = tk;
        key_len = SHA256_DIGEST_SIZE;
    }
    memcpy(k_ipad, key, key_len);
    memset(k_ipad + key_len, 0, SHA256_BLOCK_SIZE - key_len);
    memcpy(k_opad, key, key_len);
    memset(k_opad + key_len, 0, SHA256_BLOCK_SIZE - key_len);
    for (i = 0; i < SHA256_BLOCK_SIZE; ++i) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5C;
    }
    sha256(k_ipad, SHA256_BLOCK_SIZE, digest);
    memcpy(k_ipad, digest, SHA256_DIGEST_SIZE);
    memcpy(k_ipad + SHA256_DIGEST_SIZE, message, message_len);
    sha256(k_ipad, SHA256_BLOCK_SIZE, digest);
    sha256(k_opad, SHA256_BLOCK_SIZE, tk);
    for (i = 0; i < SHA256_DIGEST_SIZE; ++i) {
        digest[i] = tk[i];
    }
}

int main() {
    const unsigned char *key = (unsigned char *)"a";
    const unsigned char *data = (unsigned char *)"a";
    unsigned char hash[SHA256_DIGEST_SIZE];

    // Codificação HMAC SHA256
    hmac_sha256(key, strlen((const char *)key), data, strlen((const char *)data), hash);

    printf("HMAC SHA256: ");
    for (int i = 0; i < SHA256_DIGEST_SIZE; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
    
    return 0;
}
