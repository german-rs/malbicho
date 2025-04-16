#ifndef RSA_ENCRYPTOR_HPP
#define RSA_ENCRYPTOR_HPP

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

// Se incluyen las dependencias necesarias
#include "../core/Constants.hpp"

// Forward declarations para evitar incluir headers pesados de criptografía
namespace crypto
{
    class RSAKey;
    class CryptoContext;
}

/**
 * @class RSAEncryptorException
 * @brief Excepción específica para errores de cifrado RSA.
 */
class RSAEncryptorException : public std::runtime_error
{
public:
    explicit RSAEncryptorException(const std::string &message) : std::runtime_error(message) {}
};

/**
 * @class RSAEncryptor
 * @brief Implementa operaciones de cifrado y descifrado usando el algoritmo RSA.
 *
 * Esta clase proporciona métodos para cifrar y descifrar datos usando el
 * algoritmo RSA, típicamente usado para cifrar claves simétricas más pequeñas.
 * Implementa el patrón RAII para la gestión segura de recursos criptográficos.
 */

class RSAEncryptor
{
private:
    // Contexto criptográfico (abstracción sobre la biblioteca de criptografía)
    std::unique_ptr<crypto::RSAKey> publicKey;
    std::unique_ptr<crypto::RSAKey> privateKey;

    // Bandera que indica si la clave privada está disponible
    bool hasPrivateKey;

    // Tamaño de la clave RSA en bits
    int keySize;

    // Métodos privados
    void initializeCryptoContext();
    void validaKeySize(int size);
    std::vector<unsigned char> processData(const std::vector<unsigned char> &data, bool encrypt);

public:
    /**
     * @brief Constructor que inicializa el encriptador solo con clave pública.
     * @param publicKeyData Datos de la clave pública en formato PEM o DER
     * @param keySize Tamaño de la clave en bits (2048, 4096, etc.)
     * @throw RSAEncryptorException si hay error al importar la clave
     */
    RSAEncryptor(const std::string &publicKeyData, int keySize = Constants::Encryption::RSA_KEY_SIZE);

    /**
     * @brief Constructor que inicializa el encriptador con par de claves.
     * @param publicKeyData Datos de la clave pública en formato PEM o DER
     * @param privateKeyData Datos de la clave privada en formato PEM o DER
     * @param keySize Tamaño de la clave en bits (2048, 4096, etc.)
     * @throw RSAEncryptorException si hay error al importar las claves
     */
    RSAEncryptor(const std::string &publicKeyData, const std::string &privateKeyData,
                 int keySize = Constants::Encryption::RSA_KEY_SIZE);

    /**
     * @brief Constructor que genera un nuevo par de claves RSA.
     * @param keySize Tamaño de la clave en bits (2048, 4096, etc.)
     * @throw RSAEncryptorException si hay error al generar las claves
     */
    explicit RSAEncryptor(int keysize = Constants::Encryption::RSA_KEY_SIZE);

    /**
     * @brief Destructor virtual.
     */
    virtual ~RSAEncryptor();

    /**
     * @brief Cifra datos usando la clave pública RSA.
     * @param plaintext Datos a cifrar
     * @return Datos cifrados en formato binario
     * @throw RSAEncryptorException si hay error durante el cifrado
     */
    std::vector<unsigned char> encrypt(const std::vector<unsigned char> &plaintext);

    /**
     * @brief Cifra datos usando la clave pública RSA.
     * @param plaintext Cadena de texto a cifrar
     * @return Datos cifrados codificados en base64
     * @throw RSAEncryptorException si hay error durante el cifrado
     */
    std::string encrypt(const std::string &plaintext);

    /**
     * @brief Descifra datos usando la clave privada RSA.
     * @param ciphertext Datos cifrados en formato binario
     * @return Datos descifrados
     * @throw RSAEncryptorException si no se tiene clave privada o hay error
     */
    std::vector<unsigned char> decrypt(const std::vector<unsigned char> &ciphertext);

    /**
     * @brief Descifra datos usando la clave privada RSA.
     * @param ciphertext Datos cifrados codificados en base64
     * @return Cadena de texto descifrada
     * @throw RSAEncryptorException si no se tiene clave privada o hay error
     */
    std::string decrypt(const std::string &ciphertext);

    /**
     * @brief Verifica si el encriptador tiene la clave privada disponible.
     * @return true si la clave privada está disponible, false en caso contrario
     */
    bool hasDecryptionCapability() const;

    /**
     * @brief Establece la clave privada para habilitar el descifrado.
     * @param privateKeyData Datos de la clave privada en formato PEM o DER
     * @return true si la clave se importó correctamente, false en caso contrario
     */
    bool setPrivateKey(const std::string &privateKeyData);

    /**
     * @brief Obtiene la clave pública en formato exportable.
     * @param format Formato de exportación ("PEM" o "DER")
     * @return Clave pública en el formato especificado
     */
    std::string getPublicKey(const std::string &format = "PEM") const;

    /**
     * @brief Obtiene el tamaño máximo de datos que se pueden cifrar.
     * @return Tamaño máximo en bytes
     */
    size_t getMaxEncryptSize() const;
};

#endif // RSA_ENCRYPTOR_HPP