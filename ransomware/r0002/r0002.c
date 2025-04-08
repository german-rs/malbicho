/**
 * @file r0002.c
 * @brief Encripta archivos individuales o todos los archivos en un directorio usando AES-256-CBC
 *
 * Compilación: clang -o r0002 r0002.c -lssl -lcrypto
 * Ejecución: ./r0002 <ruta_al_directorio_o_archivo>
 *
 * @author
 * Germán Riveros S.
 * @date 06/04/2025
 * @version 1.1
 */

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <unistd.h>
#include <ftw.h>
#include <sys/stat.h>
#include <libgen.h>

#define KEY_LENGTH 32
#define IV_LENGTH 16
#define SALT_LENGTH 8
#define ITERATIONS 10000
#define BUFFER_SIZE 1024

const char *clave_secreta = "miClaveSecreta";
const char *output_base = "output";

// Manejo de errores
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

// Encriptar archivo
int encrypt_file(const char *input_file, const char *output_file, const char *key)
{
    FILE *infile = fopen(input_file, "rb");
    if (!infile)
    {
        perror("Error al abrir el archivo de entrada");
        return -1;
    }

    FILE *outfile = fopen(output_file, "wb");
    if (!outfile)
    {
        perror("Error abriendo archivo de salida");
        fclose(infile);
        return -1;
    }

    unsigned char salt[SALT_LENGTH];
    if (!RAND_bytes(salt, sizeof(salt)))
    {
        handleErrors();
    }
    fwrite(salt, 1, sizeof(salt), outfile);

    unsigned char key_derivada[KEY_LENGTH];
    unsigned char iv[IV_LENGTH];

    if (PKCS5_PBKDF2_HMAC_SHA1(key, strlen(key), salt, sizeof(salt), ITERATIONS, sizeof(key_derivada), key_derivada) != 1)
    {
        handleErrors();
    }

    if (!RAND_bytes(iv, sizeof(iv)))
    {
        handleErrors();
    }
    fwrite(iv, 1, sizeof(iv), outfile);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        handleErrors();
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key_derivada, iv) != 1)
    {
        handleErrors();
    }

    unsigned char buffer[BUFFER_SIZE];
    unsigned char ciphertext[BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH];
    int len, ciphertext_len;

    while ((len = fread(buffer, 1, sizeof(buffer), infile)) > 0)
    {
        if (EVP_EncryptUpdate(ctx, ciphertext, &ciphertext_len, buffer, len) != 1)
        {
            handleErrors();
        }
        fwrite(ciphertext, 1, ciphertext_len, outfile);
    }

    if (EVP_EncryptFinal_ex(ctx, ciphertext, &ciphertext_len) != 1)
    {
        handleErrors();
    }
    fwrite(ciphertext, 1, ciphertext_len, outfile);

    EVP_CIPHER_CTX_free(ctx);
    fclose(infile);
    fclose(outfile);

    return 0;
}

// Callback para cada archivo