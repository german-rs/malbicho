## Experimento r0002

### Descripción

Este script en **Zsh** comprime un directorio en un archivo **TAR** y lo cifra utilizando **AES-256-CBC** con _salting_ y **PBKDF2**, asegurando una clave derivada más resistente contra ataques de fuerza bruta y tablas arcoíris. Tras el cifrado, elimina los archivos originales para reforzar la seguridad.

### Funcionamiento

- Verifica la existencia del directorio antes de proceder.
- Genera un archivo **TAR** que contiene el contenido del directorio especificado.
- Cifra el archivo comprimido usando **OpenSSL** con parámetros de seguridad avanzados.
- Elimina tanto el directorio original como el archivo **TAR** tras la encriptación.
- Genera un archivo cifrado como salida, cuyo nombre está definido en el script.

### Advertencia

- Este proceso elimina permanentemente el directorio original después del cifrado.
- Si la clave de cifrado se pierde, el archivo **NO** podrá ser recuperado.

**Diseñado únicamente con fines educativos y para entornos controlados.**
