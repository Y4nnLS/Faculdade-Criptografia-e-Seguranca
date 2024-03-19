#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>

// Estrutura de usuário
typedef struct {
    char username[50];
    unsigned char password_hash[SHA256_DIGEST_LENGTH];
} User;

// Banco de dados simulado
User users_db[10];
int num_users = 0;

// Função para gerar hash SHA256 de uma senha
void generate_password_hash(const char *password, unsigned char *hash) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hash, &sha256);
}

// Função para verificar se a senha está correta
int check_password(const char *password, const unsigned char *hash) {
    unsigned char password_hash[SHA256_DIGEST_LENGTH];
    generate_password_hash(password, password_hash);
    return memcmp(password_hash, hash, SHA256_DIGEST_LENGTH) == 0;
}

// Função para gerar JWT
char *generate_jwt(const char *username) {
    // Aqui você deve implementar a geração do token JWT
    // Utilize bibliotecas de terceiros ou implemente o algoritmo de geração do JWT manualmente
    // Por questões de simplicidade, este exemplo não inclui a implementação completa do JWT
    // O JWT geralmente contém informações como o nome do usuário e um timestamp de expiração
    return NULL;
}

// Função para autenticar um usuário
char *authenticate_user(const char *username, const char *password) {
    for (int i = 0; i < num_users; ++i) {
        if (strcmp(users_db[i].username, username) == 0) {
            if (check_password(password, users_db[i].password_hash)) {
                return generate_jwt(username);
            } else {
                return NULL;
            }
        }
    }
    return NULL;
}

int main() {
    // Adicione alguns usuários ao banco de dados simulado para teste
    strcpy(users_db[num_users].username, "usuario1");
    generate_password_hash("senha123", users_db[num_users].password_hash);
    num_users++;

    // Simulação de login
    const char *username = "usuario1";
    const char *password = "senha123";
    char *jwt_token = authenticate_user(username, password);
    if (jwt_token != NULL) {
        printf("Usuário autenticado. Token JWT: %s\n", jwt_token);
        free(jwt_token);
    } else {
        printf("Falha na autenticação do usuário.\n");
    }

    return 0;
}
