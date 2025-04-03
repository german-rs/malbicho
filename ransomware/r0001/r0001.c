/**
 * @file file_encryption.c
 * @brief Implementación de encriptación de archivos usando AES-256-CBC
 *
 * Este programa proporciona funcionalidad para encriptar archivos utilizando
 * el algoritmo AES-256 en modo CBC con derivación de claves mediante PBKDF2.
 * Utiliza la biblioteca OpenSSL para las operaciones criptográficas.
 *
 * @author Germán Riveros S.
 * @date 02/04/2025
 * @version 1.0
 *
 * Compilación: clang -o r0001 r0001.c -lssl -lcrypto
 * Ejecución: ./r0001
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <unistd.h>

#define KEY_LENGTH 32    // AES-256 utiliza una clave de 256 bits (32 bytes)
#define IV_LENGTH 16     // AES utiliza un IV de 16 bytes en modo CBC
#define SALT_LENGTH 8    // Tamaño estándar para el salt PKCS5_PBKDF2
#define ITERATIONS 10000 // Número de iteraciones para PBKDF2
#define BUFFER_SIZE 1024 // Tamaño del buffer para lectura/escritura

/**
 * @brief Maneja errores de OpenSSL mostrando mensajes detallados y abortando la ejecución
 */
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

/**
 * @brief Encripta un archivo utilizando AES-256-CBC
 *
 * Esta función toma un archivo de entrada, lo encripta utilizando AES-256 en modo CBC
 * con una clave derivada mediante PBKDF2 a partir de la contraseña proporcionada,
 * y guarda el resultado en un archivo de salida.
 *
 * El formato del archivo de salida es:
 * [SALT (8 bytes)][IV (16 bytes)][DATOS ENCRIPTADOS]
 *
 * @param input_file Ruta al archivo a encriptar
 * @param output_file Ruta donde se guardará el archivo encriptado
 * @param key Contraseña utilizada para derivar la clave de encriptación
 * @return 0 en caso de éxito, -1 en caso de error
 */

int encrypt_file(const char *input_file, const char *output_file, const char *key)
{
    FILE *infile = fopen(input_file, "rb");
    if (!infile)
    {
        perror("Error al abrir archivo de entrada");
        return -1;
    }

    FILE *outfile = fopen(output_file, "wb");
    if (!outfile)
    {
        perror("Error abriendo archivo de salida");
        fclose(infile);
        return -1;
    }

    // Generar salt aleatorio para derivación de clave
    unsigned char salt[SALT_LENGTH];
    if (!RAND_bytes(salt, sizeof(salt)))
    {
        handleErrors();
    }

    // Escribir el salt en el archivo de salida
    fwrite(salt, 1, sizeof(salt), outfile);

    // Derivar la clave a partir de la contraseña utilizando PBKDF2-HMAC-SHA1
    unsigned char key_derivada[KEY_LENGTH];
    unsigned char iv[IV_LENGTH];
    if (PKCS5_PBKDF2_HMAC_SHA1(key, strlen(key), salt, sizeof(salt), ITERATIONS, sizeof(key_derivada), key_derivada) != 1)
    {
        handleErrors();
    }

    // Generar vector de inicialización (IV) aleatorio para AES-CBC
    if (!RAND_bytes(iv, sizeof(iv)))
    {
        handleErrors();
    }

    // Escribir el IV en el archivo de salidad
    fwrite(iv, 1, sizeof(iv), outfile);

    // Configurar el contexto de la encriptación
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        handleErrors();
    }

    // Inicializar el contexto con el algoritmo, clave e IV
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key_derivada, iv) != 1)
    {
        handleErrors();
    }

    // Buffers para procesar datos de entrada y salida
    unsigned char buffer[BUFFER_SIZE];
    unsigned char ciphertext[BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH]; // Buffer extra para padding
    int len;                                                      // Bytes leídos del archivo
    int ciphertext_len;                                           // Bytes producidos por la encriptación

    // Leer, encriptar y escribir el contenido por bloques
    while ((len = fread(buffer, 1, sizeof(buffer), infile)) > 0)
    {
        if (EVP_EncryptUpdate(ctx, ciphertext, &ciphertext_len, buffer, len) != 1)
        {
            handleErrors();
        }
        fwrite(ciphertext, 1, ciphertext_len, outfile);
    }

    // Finalizar la encriptación (procesar cualquier bloque restante y padding)
    if (EVP_EncryptFinal_ex(ctx, ciphertext, &ciphertext_len) != 1)
    {
        handleErrors();
    }
    fwrite(ciphertext, 1, ciphertext_len, outfile);

    // Limpiar recursos
    EVP_CIPHER_CTX_free(ctx);
    fclose(infile);
    fclose(outfile);

    return 0;
}

/**
 * @brief Función principal que ejecuta el proceso de encriptación
 *
 * Verifica la existencia del archivo de entrada y realiza la encriptación
 * utilizando una clave predefinida.
 *
 * @return 0 en caso de éxito, 1 en caso de error
 */

int main()
{
    const char *archivo = "documento.txt";
    const char *archivo_encriptado = "documento_encriptado.txt";
    const char *clave_secreta = "miClaveSecreta";

    // Verificar si el archivo existe antes de intentar encriptarlo
    if (access(archivo, F_OK) != -1)
    {
        printf("Encriptando %s...\n", archivo);

        if (encrypt_file(archivo, archivo_encriptado, clave_secreta) == 0)
        {
            printf("Archivo encriptado guardado como %s\n", archivo_encriptado);
        }
        else
        {
            printf("El archivo %s no existe. Asegúrate de que esté en el mismo directorio.\n", archivo);
        }
    }
}