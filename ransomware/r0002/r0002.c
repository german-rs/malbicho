/**
 * @file r0002.c
 * @brief Encripta archivos individuales o todos los archivos en un directorio usando AES-256-CBC
 *
 * Este programa implementa un sistema de encriptación de archivos utilizando el algoritmo AES-256-CBC.
 * Puede procesar tanto archivos individuales como directorios completos, manteniendo la estructura
 * de directorios en la salida. Utiliza derivación de claves PBKDF2 para mayor seguridad.
 *
 * @note La clave secreta y la ruta de salida están hardcodeadas en las constantes del programa
 *
 * Compilación: clang -std=c99 -D_DARWIN_C_SOURCE -o r0002 r0002.c -lssl -lcrypto
 * Ejecución: ./r0002 <ruta_al_directorio_o_archivo>
 *
 * @author Germán Riveros S.
 * @date 06/04/2025
 * @version 1.1
 */

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <unistd.h>
#include <ftw.h>
#include <sys/stat.h>
#include <libgen.h>

/**
 * @def KEY_LENGTH
 * @brief Longitud de la clave AES-256 en bytes
 */
#define KEY_LENGTH 32

/**
 * @def IV_LENGTH
 * @brief Longitud del vector de inicialización en bytes
 */
#define IV_LENGTH 16

/**
 * @def SALT_LENGTH
 * @brief Longitud del salt para PBKDF2 en bytes
 */
#define SALT_LENGTH 8

/**
 * @def ITERATIONS
 * @brief Número de iteraciones para el algoritmo PBKDF2
 */
#define ITERATIONS 10000

/**
 * @def BUFFER_SIZE
 * @brief Tamaño del buffer para lectura/escritura de archivos
 */
#define BUFFER_SIZE 1024

/** @brief Clave secreta utilizada para la encriptación */
const char *clave_secreta = "miClaveSecreta";

/** @brief Directorio base para los archivos de salida encriptados */
const char *output_base = "output";

/**
 * @brief Maneja los errores de OpenSSL
 *
 * Imprime los errores de OpenSSL al stderr y termina la ejecución del programa
 * @note Esta función no retorna, termina la ejecución del programa
 */
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

/**
 * @brief Encripta un archivo usando AES-256-CBC
 *
 * Esta función realiza la encriptación de un archivo usando AES-256-CBC con las siguientes características:
 * - Genera un salt aleatorio para PBKDF2
 * - Deriva una clave usando PBKDF2-HMAC-SHA1
 * - Genera un IV aleatorio
 * - Encripta el contenido del archivo
 *
 * @param input_file Ruta del archivo a encriptar
 * @param output_file Ruta donde se guardará el archivo encriptado
 * @param key Clave secreta utilizada para la derivación de la clave de encriptación
 * @return 0 en caso de éxito, -1 en caso de error
 */
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

/**
 * @brief Función callback para procesar cada archivo encontrado
 *
 * Esta función es llamada por nftw() para cada archivo encontrado durante
 * el recorrido del directorio. Procesa solo archivos regulares (no directorios)
 * y mantiene la estructura de directorios en la salida.
 *
 * @param path Ruta del archivo actual
 * @param sb Información del archivo (stat)
 * @param typeflag Tipo de archivo (FTW_F para archivos regulares)
 * @param ftwbuf Estructura con información adicional de nftw
 * @return 0 para continuar el recorrido, -1 en caso de error
 */
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

/**
 * @brief Función principal del programa
 *
 * Procesa los argumentos de línea de comando y maneja la lógica principal:
 * - Verifica que se proporcione una ruta como argumento
 * - Determina si la ruta es un archivo o directorio
 * - Procesa el archivo individual o recorre el directorio según corresponda
 *
 * @param argc Número de argumentos
 * @param argv Vector de argumentos. argv[1] debe ser la ruta al archivo o directorio
 * @return 0 en caso de éxito, 1 en caso de error
 */
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