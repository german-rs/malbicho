/**
 * @file shellcode_example.c
 * @brief Implementación de un programa demostrativo de shellcode
 * @author german-rs
 * @date 2025-05-14
 *
 * Este programa demuestra la creación y ejecución de shellcode
 * que imprime "Hello, World!" y termina de manera segura. Muestra
 * el mapeo de memoria, ejecución de código y interacciones de bajo
 * nivel con el sistema.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

/**
 * @brief Bytes de shellcode para imprimir "Hello, World!" y realizar una salida limpia
 *
 * El shellcode está compuesto por instrucciones en código máquina x86_64 que:
 * 1. Llama a write(1, "Hello, World!\n", 14)
 * 2. Llama a exit(0)
 *
 * Disposición de memoria y números de syscall:
 * - Número de syscall write: 1
 * - Número de syscall exit: 60
 * - Descriptor de archivo stdout: 1
 */
unsigned char shellcode[] = {
    // write(1, "Hello, World!\n", 14)
    0x48, 0xc7, 0xc0, 0x01, 0x00, 0x00, 0x00, // mov rax, 1 (syscall write)
    0x48, 0xc7, 0xc7, 0x01, 0x00, 0x00, 0x00, // mov rdi, 1 (stdout)
    0x48, 0xb8, 0x48, 0x65, 0x6c, 0x6c, 0x6f, // movabs rax, "Hello, W"
    0x2c, 0x20, 0x57,
    0x50,                                     // push rax
    0x48, 0xb8, 0x6f, 0x72, 0x6c, 0x64, 0x21, // movabs rax, "orld!\n"
    0x0a, 0x00, 0x00,
    0x50,                                     // push rax
    0x48, 0x89, 0xe6,                         // mov rsi, rsp (buffer)
    0x48, 0xc7, 0xc2, 0x0e, 0x00, 0x00, 0x00, // mov rdx, 14 (length)
    0x0f, 0x05,                               // syscall

    // exit(0)
    0x48, 0x31, 0xff,                         // xor rdi, rdi
    0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00, // mov rax, 60 (syscall exit)
    0x0f, 0x05                                // syscall
};

/**
 * @brief Imprime un volcado hexadecimal de los datos proporcionados
 *
 * Esta función muestra el contenido de un buffer de memoria en formato hexadecimal,
 * 16 bytes por línea. Cada byte se muestra como un número hexadecimal de dos dígitos.
 *
 * @param data Puntero al buffer de datos a mostrar
 * @param len Longitud del buffer de datos en bytes
 */
void hexdump(const unsigned char *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
}

/**
 * @brief Punto de entrada del programa
 *
 * La función principal realiza las siguientes operaciones:
 * 1. Muestra el tamaño y contenido del shellcode
 * 2. Mapea memoria ejecutable usando mmap
 * 3. Copia el shellcode a la memoria ejecutable
 * 4. Ejecuta el shellcode
 *
 * @return int Devuelve 0 en caso de éxito, 1 en caso de error
 */
int main()
{
    /* Obtener tamaño del shellcode */
    size_t len = sizeof(shellcode);

    /* Mostrar información del shellcode */
    printf("[+] Tamaño del shellcode: %zu bytes\n", len);
    printf("[+] Contenido del shellcode:\n");
    hexdump(shellcode, len);

    /* Mapear memoria ejecutable
     * PROT_READ|PROT_WRITE|PROT_EXEC: Memoria con permisos de lectura, escritura y ejecución
     * MAP_PRIVATE|MAP_ANONYMOUS: Mapeo privado no respaldado por ningún archivo
     */
    void *mem = mmap(NULL, len,
                     PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    /* Verificar si el mapeo de memoria falló */
    if (mem == MAP_FAILED)
    {
        perror("[-] Error en mmap");
        return 1;
    }

    /* Mostrar dirección de memoria mapeada y copiar shellcode */
    printf("[+] Memoria ejecutable mapeada en: %p\n", mem);
    memcpy(mem, shellcode, len);

    /* Ejecutar shellcode */
    printf("[+] Ejecutando shellcode...\n");
    fflush(stdout);

    /**
     * @brief Explicación detallada del casteo y ejecución del shellcode
     *
     * Desglose de la expresión: ((void (*)())mem)();
     *
     * 1. (void (*)()) - Es una declaración de tipo de puntero a función
     *    - void: indica que la función no retorna valor
     *    - (*): indica que es un puntero
     *    - (): sin parámetros
     *
     * 2. mem - El puntero a la memoria donde está el shellcode
     *
     * 3. () - Los paréntesis finales ejecutan la función
     */
    ((void (*)())mem)();

    return 0;
}