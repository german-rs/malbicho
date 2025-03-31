#! /bin/zsh

# Archivo de entrada.
archivo="documento.txt"

# Archivo de salida (encriptado).
archivo_encriptado="documento_encriptado.txt"

# Clave secreta para la encriptación.
clave_secreta="miClaveSecreta"

# Verificar que el archivo de entrada existe.
if [[ -f "$archivo" ]]; then
    echo "Encriptando $archivo..."
    
    # openssl enc: Llama a la funcionalidad de encriptación.
    # -aes-256-cbc: Algoritmo de encriptación AES con longitud de 256, modo de operación cbc (cipher block chaining).
    # -salt: Agrega un valor aleatorio al proceso de encriptación. Previene ataques Rainbow Tables.
    # -pbkdf2 (Password-Based Key Derivation Function 2): Se utiliza para derivar una clave segura desde la contraseña. Previene atques por fuerza bruta.
    # -iter 100000: Número de iteraciones que realiza el algoritmo pbkdf2 al derivar la clave.
    # -in "$archivo": Define el archivo de entrada que será encriptado.  
    # -out "$archivo_encriptado": El archivo donde se guardara el resultado de la encriptación.
    # -pass pass:"$clave_secreta": Proporciona la contraseña para la encriptación.

    openssl enc -aes-256-cbc -salt -pbkdf2 -iter 100000 -in "$archivo" -out "$archivo_encriptado" -pass pass:"$clave_secreta"
    echo "Archivo encriptado guardado como $archivo_encriptado"
else
    "El archivo $archivo no existe. Asegurate de que esté en el mismo directorio"
fi