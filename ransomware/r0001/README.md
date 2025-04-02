# Experimento r0001

### Descripción

Este script en Bash utiliza OpenSSL para encriptar un archivo de texto con el algoritmo **AES-256-CBC**. Implementa medidas de seguridad como el uso de _salting_ y **PBKDF2** con 100,000 iteraciones para fortalecer la clave derivada.

### Funcionamiento

- Verifica la existencia del archivo de entrada antes de proceder con la encriptación.
- Utiliza OpenSSL con parámetros de seguridad mejorados para evitar ataques de fuerza bruta y tablas arcoíris.
- Genera un archivo encriptado como salida, con un nombre definido en el script.

### Advertencia

- Este script está diseñado con fines educativos y debe utilizarse en entornos controlados. El uso inadecuado de técnicas de cifrado puede llevar a la pérdida de acceso a los datos si la clave se extravía.
