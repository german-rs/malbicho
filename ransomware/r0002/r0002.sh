#! /bin/zsh

# 
# Script de compresión y cifrado de directorios con OpenSSL
#
# Descripción:
#   Este script en Zsh comprime un directorio en un archivo TAR y lo cifra 
#   utilizando el algoritmo AES-256-CBC con PBKDF2 para mejorar la seguridad.
#   Tras el cifrado, elimina el directorio original y el archivo temporal TAR.
#
# Uso:
#   - Asegúrese de que el directorio especificado en la variable "DIRECTORIO" exista.
#   - Ejecute el script en un entorno compatible con Zsh.
#
# Parámetros:
#   - DIRECTORIO: Nombre del directorio a comprimir y cifrar.
#   - ARCHIVO_CIFRADO: Nombre del archivo resultante después del cifrado.
#   - CLAVE: Clave utilizada para la encriptación (definida en el script).
#
# Advertencia:
#   - Este proceso elimina el directorio original tras el cifrado. 
#   - Asegúrese de recordar la clave, ya que sin ella no podrá descifrar el archivo.
#
#
# Autor: Germán Riveros S.
# Fecha: 01/04/2025
# 

DIRECTORIO="carpeta"
ARCHIVO_CIFRADO="carpeta.tar.enc"
CLAVE="ClaveSuperSecreta"

# Verifica si el directorio existe.
if [[ -d "$DIRECTORIO" ]]; then
    echo "[+] Comprimiendo el directorio..."
    tar -cf carpeta.tar "$DIRECTORIO"

    echo "[+] Cifrando con AES-256 con PBKDF2..."

    # Cifra el archivo 'carpeta.tar' usando el algoritmo AES-256 en modo CBC, con una clave derivada mediante PBKDF2.
    # La opción '-salt' añade una sal aleatoria para mejorar la seguridad y evitar ataques de diccionario.
    # El archivo cifrado se guarda en la variable "$ARCHIVO_CIFRADO".
    # La clave de cifrado se proporciona a través de la variable "$CLAVE".
    openssl enc -aes-256-cbc -salt -pbkdf2 -in carpeta.tar -out "$ARCHIVO_CIFRADO" -pass pass:"$CLAVE"

    echo "[+] Eliminando archivos originales..."
    rm -rf "$DIRECTORIO" carpeta.tar

    echo "[✔] Cifrado completado: $ARCHIVO_CIFRADO"
else
    echo "[!] Error: No se encontró el directorio '$DIRECTORIO'"
fi
