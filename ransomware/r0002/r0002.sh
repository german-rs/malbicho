#! /bin/zsh

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



