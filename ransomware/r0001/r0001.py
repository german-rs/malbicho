"""
@file r0001.py
@brief Implementación de encriptación de archivos usando AES-256-CBC

Este programa proporciona funcionalidad para encriptar archivos utilizando
el algoritmo AES-256 en modo CBC con derivación de claves mediante PBKDF2.
Utiliza la biblioteca cryptography para las operaciones criptográficas.

Ejecución: python r0001.py

@author Basado en código de Germán Riveros S.
@date 03/04/2025
@version 1.0
"""

import os
import sys
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.backends import default_backend

# Constantes
KEY_LENGTH = 32     # AES-256 utiliza una clave de 32 bytes
IV_LENGTH = 16      # AES utiliza un IV de 16 bytes en modo CBC
SALT_LENGTH = 8     # Tamaño estándar para el salt PBKDF2
ITERATIONS = 100000 # Número de iteraciones para PBKDF2
BUFFER_SIZE = 1024  # Tamaño del buffer para lectura/escritura

def encrypt_file(input_file, output_file, key):
        """
    Encripta un archivo utilizando AES-256-CBC

    Esta función toma un archivo de entrada, lo encripta utilizando AES-256 en modo CBC
    con una clave derivada mediante PBKDF2 a partir de la contraseña proporcionada,
    y guarda el resultado en un archivo de salida.

    El formato del archivo de salida es:
    [SALT (8 bytes)][IV (16 bytes)][DATOS ENCRIPTADOS]

    Args:
        input_file (str): Ruta al archivo a encriptar
        output_file (str): Ruta donde se guardará el archivo encriptado
        key (str): Contraseña utilizada para derivar la clave de encriptación

    Returns:
        int: 0 en caso de éxito, -1 en caso de error
    """

    
    try:
        # Abrir los archivos
        with open(input_file, 'rb') as infile:
            with open(output_file, 'wb') as outfile:

                # Generar salt aleatorio para derivación de clave
                salt = os.urandom(SALT_LENGTH)

                # Escribir el salt en el archivo de salida
                outfile.write(salt)

                # Derivar la clave a partir de la contraseña utilizando PBKDF2-HMAC-SHA1
                kdf = PBKDF2HMAC(
                    algorithm=hashes.SHA1(),
                    length=KEY_LENGTH,
                    salt=salt,
                    iterations=ITERATIONS,
                    backend=default_backend()
                )
                key_derivada = kdf.derive(key.encode())

                # Generar vector de inicialización (IV) aleatorio para AES-CBC
                iv = os.urandom(IV_LENGTH)

                # Escribir el IV en el archivo de salida
                outfile.write(iv)

                # Configurar el cifrado AES-256-CBC
                cipher = Cipher(
                    algorithms.AES(key_derivada),
                    modes.CBC(iv),
                    backend=default_backend()
                )
                encryptor = cipher.encryptor()

                # Leer, encriptar y escribir el contenido por bloques
                while True:
                    chunk = infile.read(BUFFER_SIZE)
                    if not chunk:
                        break

                    # Si este es el último bloque, añadir padding PKCS#7
                    if len(chunk) < BUFFER_SIZE:
                        padding_length = 16 - (len(chunk) % 16)
                        chunk += bytes([padding_length]) * padding_length

                    ciphertext = encryptor.update(chunk)
                    outfile.write(ciphertext)

                # Finalizar la encriptación
                ciphertext = encryptor.finalize()
                if ciphertext:
                    outfile.write(ciphertext)
            

        return 0

    except Exception as e:
        print(f"Error durante la encriptación: {e}")
        return -1


def main():
    """
    Función principal que ejecuta el proceso de encriptación

    Verifica la existencia del archivo de entrada y realiza la encriptación
    utilizando una clave predefinida.

    Returns:
        int: 0 en caso de éxito, 1 en caso de error
    """  
    archivo = "documento.txt"
    archivo_encriptado = "documento_encriptado.txt"
    clave_secreta = "miClaveSecreta"

    # Verificar si el archivo existe antes de intentar encriptarlo
    if os.path.exists(archivo):
        print(f"Encriptando {archivo}...")

        if encrypt_file(archivo, archivo_encriptado, clave_secreta) == 0:
            print(f"Archivo encriptado guardado como {archivo_encriptado}")
            return 0
        else:
            return 1
    else:
        print(f"El archivo {archivo} no existe. Asegúrate de que esté en la misma carpeta.")
        return 1

if __name__ == "__main__":
    sys.exit(main())

