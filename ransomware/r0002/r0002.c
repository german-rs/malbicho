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

int process_file(const char *path, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if (typeflag == FTW_F)
    {
        // Ruta relativa desde el punto base
        char relative_path[PATH_MAX];
        snprintf(relative_path, sizeof(relative_path), "%s", path + ftwbuf->base);

        // Ruta de salida: output/<ruta_relativa>.enc
        char output_path[PATH_MAX];
        snprintf(output_path, sizeof(output_path), "%s/%s.enc", output_base, relative_path);

        // Crear directorios necesarios
        char dir_copy[PATH_MAX];
        strcpy(dir_copy, output_path);
        char *dir = dirname(dir_copy);
        char mkdir_cmd[PATH_MAX + 10];
        snprintf(mkdir_cmd, sizeof(mkdir_cmd), "mkdir -p \"%s\"", dir);
        system(mkdir_cmd);

        printf("Cifrando: %s -> %s\n", path, output_path);
        if (encrypt_file(path, output_path, clave_secreta) != 0)
        {
            fprintf(stderr, "Error al cifrar %s\n", path);
        }
    }

    return 0;
}

// Main

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <archivo_o_directorio>\n", argv[0]);
        return 1;
    }

    // Crear carpeta de salida si no existe
    mkdir(output_base, 0755);

    struct stat path_stat;
    if (stat(argv[1], &path_stat) != 0)
    {
        perror("Error al acceder a la ruta");
        return 1;
    }

    if (S_ISDIR(path_stat.st_mode))
    {
        printf("Recorriendo directorio: %s\n", argv[1]);
        if (nftw(argv[1], process_file, 20, FTW_PHYS) == -1)
        {
            perror("nftw");
            return 1;
        }
    }
    else if (S_ISREG(path_stat.st_mode))
    {
        // Cifrar un solo archivo
        char output_path[PATH_MAX];
        snprintf(output_path, sizeof(output_path), "%s/%s.enc", output_base, basename(argv[1]));
        if (encrypt_file(argv[1], output_path, clave_secreta) != 0)
        {
            fprintf(stderr, "Error al cifrar el archivo\n");
            return 1;
        }
        printf("Archivo cifrado: %s\n", output_path);
    }
    else
    {
        fprintf(stderr, "La ruta no es válida\n");
        return 1;
    }
    return 0;
}