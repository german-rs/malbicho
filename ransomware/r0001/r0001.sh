#! /bin/zsh


# @file r0001.sh
# @brief Script de encriptación con OpenSSL
#
# @details Este script en Zsh encripta un archivo de texto utilizando el algoritmo 
# AES-256-CBC a través de OpenSSL. Implementa medidas de seguridad como salting
# y PBKDF2 con 100,000 iteraciones para fortalecer la clave derivada.
#
# @section usage Uso
# - Asegurarse de que el archivo "documento.txt" esté en el mismo directorio.
# - Ejecutar el script en un entorno compatible con Zsh.
#
# @section params Parámetros
# - archivo: Nombre del archivo de entrada a encriptar.
# - archivo_encriptado: Nombre del archivo de salida con el contenido cifrado.
# - clave_secreta: Clave utilizada para la encriptación (definida en el script).
# 
# @warning El uso inadecuado de la clave de encriptación puede llevar a la pérdida 
# irreversible del acceso a los datos.
#   
# @author Germán Riveros S.
# @date 01/04/2025
# @version 1.0
# 

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
    echo "El archivo $archivo no existe. Asegúrate de que esté en el mismo directorio."
fi