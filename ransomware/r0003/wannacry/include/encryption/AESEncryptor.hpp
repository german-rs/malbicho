/**
 * @file AESEncryptor.hpp
 * @brief Clase para el cifrado y descifrado de archivos usando AES.
 *
 * Esta clase proporciona funcionalidades para el cifrado y descifrado de archivos
 * utilizando el algoritmo AES (Advanced Encryption Standard) a través de la
 * biblioteca OpenSSL.
 */

#ifndef AES_ENCRYPTOR_HPP
#define AES_ENCRYPTOR_HPP

#include <vector>
#include <string>
#include <memory>
#include <openssl/evp.h>

/**
 * @class AESEncryptor
 * @brief Implementa funcionalidades de cifrado y descifrado AES.
 *
 * Permite cifrar y descifrar archivos usando diferentes modos de operación
 * de AES con soporte para especificar extensiones de archivo objetivo y
 * algoritmos de cifrado específicos.
 */
class AESEncryptor
{
private:
    /** @brief Contexto de cifrado de OpenSSL. */
    EVP_CIPHER_CTX *ctx;

    /** @brief Lista de extensiones de archivo que serán objetivo de cifrado. */
    std::vector<std::string> targetExtensions;

    /** @brief Algoritmo de cifrado a utilizar (ej. AES-256-CBC). */
    std::string encryptionAlgorithm;

    /** @brief Indica si se ha establecido una clave de cifrado. */
    bool keySet;

    /**
     * @brief Verifica si un archivo tiene una extensión objetivo para cifrado.
     *
     * @param filePath Ruta del archivo a verificar.
     * @return bool true si la extensión está en la lista de extensiones objetivo.
     */
    bool checkFileExtension(const std::string &filePath);

    /**
     * @brief Renombra un archivo después del cifrado/descifrado.
     *
     * @param originalPath Ruta original del archivo.
     */
    void renameFile(const std::string &originalPath);

public:
    /**
     * @brief Constructor por defecto.
     *
     * Inicializa el contexto de cifrado y establece valores predeterminados.
     */
    AESEncryptor();

    /**
     * @brief Destructor.
     *
     * Libera los recursos asociados al contexto de cifrado.
     */
    ~AESEncryptor();

    /**
     * @brief Genera una clave aleatoria para el cifrado.
     *
     * Crea una nueva clave aleatoria segura para usar en operaciones de cifrado.
     */
    void generateRandomKey();

    /**
     * @brief Cifra un archivo especificado.
     *
     * @param filePath Ruta del archivo a cifrar.
     * @return bool true si el cifrado fue exitoso, false en caso contrario.
     */
    bool encryptFile(const std::string &filePath);

    /**
     * @brief Establece la clave y vector de inicialización para el cifrado.
     *
     * @param key Vector de bytes que contiene la clave de cifrado.
     * @param iv Vector de bytes que contiene el vector de inicialización.
     */
    void setKey(const std::vector<unsigned char> &key,
                const std::vector<unsigned char> &iv);

    /**
     * @brief Descifra datos cifrados previamente.
     *
     * @param ciphertext Vector de bytes con el contenido cifrado.
     * @return bool true si el descifrado fue exitoso, false en caso contrario.
     */
    bool decryptFile(const std::vector<unsigned char> &ciphertext);

    /**
     * @brief Establece las extensiones de archivo objetivo para cifrado.
     *
     * @param extensions Vector con las extensiones de archivo a considerar.
     */
    void setTargetExtensions(const std::vector<std::string> &extensions);

    /**
     * @brief Establece el algoritmo de cifrado a utilizar.
     *
     * @param algorithm Nombre del algoritmo (ej. "AES-256-CBC").
     */
    void setEncryptionAlgorithm(const std::string &algorithm);
};

#endif // AES_ENCRYPTOR_HPP