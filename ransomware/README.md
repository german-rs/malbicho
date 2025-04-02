# Ransomware

El ransomware es un tipo de malware diseñado para cifrar los archivos de un usuario y bloquear el acceso a ellos hasta que se realice un pago, conocido como "rescate". Este software malicioso suele propagarse a través de correos electrónicos de phishing, que contienen enlaces o archivos adjuntos fraudulentos, descargas de contenido infectado desde sitios web no seguros, o mediante la explotación de vulnerabilidades en el software o sistema operativo.

Una vez que el ransomware infecta un dispositivo, cifra los archivos más importantes, haciéndolos inaccesibles para el usuario. Posteriormente, los atacantes presentan un mensaje en el que exigen el pago del rescate, generalmente en criptomonedas como Bitcoin, para garantizar la anonimidad de la transacción. Sin embargo, incluso si se realiza el pago, no existe garantía de que los ciberdelincuentes cumplan con su promesa de descifrar los datos.

| Experimento                                                               | Descripción                               |
| ------------------------------------------------------------------------- | ----------------------------------------- |
| [r0001](https://github.com/german-rs/malbicho/tree/main/ransomware/r0001) | Se implementa la propiedad autoreplicante |
| [r0002](https://github.com/german-rs/malbicho/tree/main/ransomware/r0002) | Se implementa la propiedad autoreplicante |

## Métodos de Cifrado en Ransomware

Un ransomware generalmente utiliza cifrado fuerte para secuestrar archivos y exigir un rescate. Los métodos más comunes incluyen:

### 🔐 1. Cifrado Asimétrico (RSA)

- Utiliza una clave pública para cifrar archivos y una clave privada (oculta en el servidor del atacante) para descifrarlos.
- **Ejemplo:** RSA-2048 o RSA-4096.
- Usado en ransomware avanzado como **WannaCry** y **Locky**.

### 🔥 2. Cifrado Simétrico (AES)

- Usa una única clave secreta para cifrar y descifrar archivos.
- **Ejemplo:** AES-256, muy rápido y seguro.
- La clave suele estar encriptada con RSA y enviada al servidor del atacante.

### 🚀 3. Cifrado Híbrido (RSA + AES)

- AES cifra los archivos (porque es rápido).
- RSA cifra la clave AES (para evitar que la víctima la descubra).
- Usado en ransomware como **TeslaCrypt** y **CryptoLocker**.
