# Estudio y análisis del Ransomware WannaCry

## **⚠️ WannaCry Replica Project (Educational Purposes Only)**

**A modular, research-focused implementation of the WannaCry ransomware architecture for academic analysis.**

### **🔍 Project Overview**

This repository contains a **simulated, non-functional replica** of the WannaCry ransomware, designed to:

- Study malware behavior in a controlled environment.
- Understand ransomware propagation techniques (e.g., SMB exploits, encryption methods).
- Demonstrate defensive strategies (kill switches, decryption workflows).

🚫 **This is NOT a real malware tool.** It lacks destructive capabilities (no actual file encryption, network attacks, or persistence mechanisms).

---

### **⚠️ Legal & Ethical Warnings**

#### **🚨 Disclaimer**

- This project is **strictly for educational, research, and cybersecurity training purposes**.
- **Replicating or modifying this code for malicious purposes is illegal.**
- The authors **do not endorse or support malware development** outside ethical hacking environments.

#### **🔐 Usage Restrictions**

- **Run only in isolated lab environments** (e.g., VirtualBox, VMware, Docker).
- **Do not deploy on real systems, networks, or devices.**
- Compliance with local laws (e.g., **Computer Fraud and Abuse Act (CFAA)** in the U.S., **GDPR** in the EU) is mandatory.

---

### **🛡️ Ethical Guidelines**

If using this project for research:

1. **Obtain explicit permission** before testing on any network.
2. **Document intent** (e.g., academic paper, malware analysis course).
3. **Report vulnerabilities responsibly** if discovered during testing.

---

### **📚 Educational Resources**

For further learning:

- [MITRE ATT&CK: Ransomware Tactics](https://attack.mitre.org/software/S0366/)
- [CISA WannaCry Analysis](https://www.cisa.gov/news-events/cybersecurity-advisories/aa17-134a)
- [Ethical Hacking Best Practices](https://www.eccouncil.org/ethical-hacking/)

---

### **📝 License**

This repository is licensed under **MIT License** (see [LICENSE](LICENSE)), which explicitly **prohibits misuse**. Violators assume all legal responsibility.

---

#### **🛑 By using this project, you agree to these terms.**

**Report misuse to:** [your-email@example.com] | [Your Institution/Lab Name]

---

Este texto:

1. **Clarifica el propósito educativo** desde el principio.
2. **Incluye advertencias legales** destacadas.
3. **Proporciona recursos** para aprendizaje ético.
4. **Desalienta el uso malicioso** con lenguaje firme pero profesional.

¿Quieres ajustar algún detalle (ej: añadir una política de reporte de bugs o créditos institucionales)?

### Estructura de directorios y archivos

```
wannacry/
├── include/ # Archivos de cabecera (.hpp)
│ ├── core/
│ │ ├── WannaCry.hpp # Clase principal
│ │ ├── Config.hpp # Configuraciones globales
│ │ └── Constants.hpp # Constantes del sistema
│ ├── encryption/
│ │ ├── FileEncryptor.hpp # Interfaz para cifrado de archivos
│ │ ├── RSAEncryptor.hpp # Implementación de cifrado RSA
│ │ ├── AESEncryptor.hpp # Implementación de cifrado AES
│ │ └── KeyGenerator.hpp # Generador de claves
│ ├── filesystem/
│ │ ├── FileScanner.hpp # Escaneo del sistema de archivos
│ │ ├── FileManager.hpp # Gestión de archivos
│ │ └── DriveEnumerator.hpp # Enumeración de unidades
│ ├── network/
│ │ ├── Propagator.hpp # Propagación por red
│ │ ├── ExploitManager.hpp # Gestor de exploits (EternalBlue)
│ │ ├── C2Communication.hpp # Comunicación con C&C
│ │ └── NetworkScanner.hpp # Escaneo de redes
│ ├── payment/
│ │ ├── PaymentHandler.hpp # Manejo de pagos
│ │ ├── BitcoinAddress.hpp # Manejo de direcciones Bitcoin
│ │ └── PaymentVerifier.hpp # Verificación de pagos
│ ├── persistence/
│ │ ├── Persistence.hpp # Interfaz de persistencia
│ │ ├── RegistryManager.hpp # Gestión del registro de Windows
│ │ ├── StartupManager.hpp # Gestión de inicio automático
│ │ └── ProcessHider.hpp # Ocultamiento de procesos
│ ├── ui/
│ │ ├── RansomUI.hpp # Interfaz de usuario
│ │ ├── MessageDisplay.hpp # Visualización de mensajes
│ │ └── PaymentInstructions.hpp # Instrucciones de pago
│ └── utils/
│ ├── Logger.hpp # Sistema de logging
│ ├── SystemInfo.hpp # Información del sistema
│ ├── AntiDebug.hpp # Técnicas anti-depuración
│ └── KillSwitch.hpp # Implementación del kill switch
│
├── src/ # Archivos de implementación (.cpp)
│ ├── core/
│ │ ├── WannaCry.cpp
│ │ └── Config.cpp
│ ├── encryption/
│ │ ├── FileEncryptor.cpp
│ │ ├── RSAEncryptor.cpp
│ │ ├── AESEncryptor.cpp
│ │ └── KeyGenerator.cpp
│ ├── filesystem/
│ │ ├── FileScanner.cpp
│ │ ├── FileManager.cpp
│ │ └── DriveEnumerator.cpp
│ ├── network/
│ │ ├── Propagator.cpp
│ │ ├── ExploitManager.cpp
│ │ ├── C2Communication.cpp
│ │ └── NetworkScanner.cpp
│ ├── payment/
│ │ ├── PaymentHandler.cpp
│ │ ├── BitcoinAddress.cpp
│ │ └── PaymentVerifier.cpp
│ ├── persistence/
│ │ ├── Persistence.cpp
│ │ ├── RegistryManager.cpp
│ │ ├── StartupManager.cpp
│ │ └── ProcessHider.cpp
│ ├── ui/
│ │ ├── RansomUI.cpp
│ │ ├── MessageDisplay.cpp
│ │ └── PaymentInstructions.cpp
│ └── utils/
│ ├── Logger.cpp
│ ├── SystemInfo.cpp
│ ├── AntiDebug.cpp
│ └── KillSwitch.cpp
│
├── resources/ # Recursos para la UI y configuración
│ ├── languages/ # Archivos de idiomas para la UI
│ │ ├── en.json # Inglés
│ │ ├── ru.json # Ruso
│ │ └── ... # Otros idiomas
│ ├── images/ # Imágenes para la UI
│ │ └── ransom_background.png
│ └── config/
│ ├── target_extensions.json # Extensiones de archivo a cifrar
│ └── server_list.json # Lista de servidores C&C
│
├── tests/ # Pruebas unitarias
│ ├── encryption_tests/
│ ├── filesystem_tests/
│ └── network_tests/
│
├── tools/ # Herramientas de compilación y despliegue
│ ├── build.sh
│ └── obfuscate.py # Script de ofuscación
│
├── CMakeLists.txt # Configuración de CMake
├── README.md # Documentación básica
└── LICENSE # Licencia (en un malware real no existiría)

```
