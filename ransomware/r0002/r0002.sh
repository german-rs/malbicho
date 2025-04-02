#! /bin/zsh

DIRECTORIO="carpeta"
ARCHIVO_CIFRADO="carpeta.tar.enc"
CLAVE="ClaveSuperSecreta"

# Verifica si el directorio existe.
if [[ -d "$DIRECTORIO" ]]; then
    echo "[+] Comprimiendo el directorio..."
    tar -cf carpeta.tar "$DIRECTORIO"

    echo "[+] Cifrando con AES-256..."
    openssl enc -aes-256-cbc -salt -pbkdf2 -in carpeta.tar -out "$ARCHIVO_CIFRADO" -pass pass:"$CLAVE"

    echo "[+] Eliminando archivos originales..."
    rm -rf "$DIRECTORIO" carpeta.tar

    echo "[✔] Cifrado completado: $ARCHIVO_CIFRADO"
else
    echo "[!] Error: No se encontró el directorio '$DIRECTORIO'"
fi



