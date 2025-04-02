#!/bin/bash

ARCHIVO_CIFRADO="carpeta.tar.enc"
CLAVE="ClaveSuperSecreta"

if [[ -f "$ARCHIVO_CIFRADO" ]]; then
    echo "[+] Descifrando archivo..."
    openssl enc -d -aes-256-cbc -in "$ARCHIVO_CIFRADO" -out carpeta.tar -pass pass:"$CLAVE"

    echo "[+] Extrayendo archivos..."
    tar -xf carpeta.tar

    echo "[+] Limpiando archivos temporales..."
    rm carpeta.tar "$ARCHIVO_CIFRADO"

    echo "[✔] Desencriptado completado"
else
    echo "[!] Error: No se encontró '$ARCHIVO_CIFRADO'"
fi
