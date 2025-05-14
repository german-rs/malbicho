#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// Shellcode para exit(0)
// En assembly sería:
//   mov eax, 60    ; syscall number para exit en x64
//   xor edi, edi   ; argument 0
//   syscall        ; llamar al sistema
unsigned char shellcode[] = {
    0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00, // mov rax, 60 (syscall exit)
    0x48, 0x31, 0xff,                         // xor rdi, rdi (argumento 0)
    0x0f, 0x05                                // syscall
};

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

int main()
{
    size_t len = sizeof(shellcode);

    printf("[+] Tamaño del shellcode: %zu bytes\n", len);
    printf("[+] Contenido del shellcode:\n");
    hexdump(shellcode, len);

    // Mapear memoria ejecutable
    void *mem = mmap(NULL, len,
                     PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (mem == MAP_FAILED)
    {
        perror("[-] Error en mmap");
        return 1;
    }

    printf("[+] Memoria ejecutable mapeada en: %p\n", mem);

    // Copiar shellcode a la memoria ejecutable
    memcpy(mem, shellcode, len);

    printf("[+] Shellcode copiado a memoria\n");
    printf("[+] Ejecutando shellcode...\n");
    fflush(stdout);

    // Ejecutar shellcode
    ((void (*)())mem)();

    // Este código nunca se ejecutará debido al exit()
    printf("[-] Este mensaje nunca se mostrará\n");

    return 0;
}