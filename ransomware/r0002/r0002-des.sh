#!/bin/bash

ARCHIVO_CIFRADO="carpeta.tar.enc"
CLAVE="ClaveSuperSecreta"

if [[ -f "$ARCHIVO_CIFRADO" ]]; then
    echo "[+] Descifrando archivo con PBKDF2..."
    
    # Descifra el archivo cifrado utilizando el algoritmo AES-256 en modo CBC.
    # La opción '-d' indica que se realizará un descifrado en lugar de un cifrado.
    # La opción '-salt' permite que OpenSSL lea la sal almacenada en el archivo cifrado y la use para derivar la clave correctamente.
    # Se emplea PBKDF2 ('-pbkdf2') para mejorar la seguridad en la derivación de la clave desde la contraseña.
    # El archivo resultante se almacena como 'carpeta.tar', recuperando los datos originales antes del cifrado.
    # La clave de descifrado es proporcionada por la variable "$CLAVE".
    openssl enc -d -aes-256-cbc -salt -pbkdf2 -in "$ARCHIVO_CIFRADO" -out carpeta.tar -pass pass:"$CLAVE"

    echo "[+] Extrayendo archivos..."
    tar -xf carpeta.tar

    echo "[+] Limpiando archivos temporales..."
    rm carpeta.tar "$ARCHIVO_CIFRADO"

    echo "[✔] Desencriptado completado"
else
    echo "[!] Error: No se encontró '$ARCHIVO_CIFRADO'"
fi
