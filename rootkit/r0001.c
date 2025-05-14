#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Función para ocultar el proceso.
void ocultar_proceso(char *argv[])
{
    // Cambiar el nombre del proceso en ps/pstree
    char *nuevo_nombre = "[kworker/0:0]";
    strncpy(argv[0], nuevo_nombre, strlen(nuevo_nombre));
    memset(argv[0] + strlen(nuevo_nombre), '\0', strlen(argv[0]) - strlen(nuevo_nombre));

    // Desvincular del terminal padre
    if (fork() != 0)
    {
        exit(0); // Terminar el proceso padre
    }

    // Crear nueva sesión para desvincular del terminal
    setsid();
    printf("Proceso en ejecución (oculto)\n");
}

int main(int argc, char *argv[])
{
    // Ocultar el proceso
    ocultar_proceso(argv);

    while (1)
    {
        sleep(10);
    }

    return 0;
}