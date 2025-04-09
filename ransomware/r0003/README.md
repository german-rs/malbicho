# Estudio y análisis del Ransomware WannaCry

## Estructura de directorios y archivos

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
