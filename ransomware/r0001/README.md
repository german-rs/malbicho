# Experimento r0001

### Descripción

Este script en Bash utiliza OpenSSL para encriptar un archivo de texto con el algoritmo **AES-256-CBC**. Implementa medidas de seguridad como el uso de _salting_ y **PBKDF2** con 100,000 iteraciones para fortalecer la clave derivada.

### Funcionamiento

- Verifica la existencia del archivo de entrada antes de proceder con la encriptación.
- Utiliza OpenSSL con parámetros de seguridad mejorados para evitar ataques de fuerza bruta y tablas arcoíris.
- Genera un archivo encriptado como salida, con un nombre definido en el script.

### Advertencia

- Este script está diseñado con fines educativos y debe utilizarse en entornos controlados. El uso inadecuado de técnicas de cifrado puede llevar a la pérdida de acceso a los datos si la clave se extravía.

---

## Notas:

### Métodos de Cifrado en Ransomware

Un ransomware generalmente utiliza cifrado fuerte para secuestrar archivos y exigir un rescate. Los métodos más comunes incluyen:

#### 🔐 1. Cifrado Asimétrico (RSA)

- Utiliza una clave pública para cifrar archivos y una clave privada (oculta en el servidor del atacante) para descifrarlos.
- **Ejemplo:** RSA-2048 o RSA-4096.
- Usado en ransomware avanzado como **WannaCry** y **Locky**.

#### 🔥 2. Cifrado Simétrico (AES)

- Usa una única clave secreta para cifrar y descifrar archivos.
- **Ejemplo:** AES-256, muy rápido y seguro.
- La clave suele estar encriptada con RSA y enviada al servidor del atacante.

#### 🚀 3. Cifrado Híbrido (RSA + AES)

- AES cifra los archivos (porque es rápido).
- RSA cifra la clave AES (para evitar que la víctima la descubra).
- Usado en ransomware como **TeslaCrypt** y **CryptoLocker**.
