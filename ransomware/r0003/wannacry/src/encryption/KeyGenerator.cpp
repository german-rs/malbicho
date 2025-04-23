#include "KeyGenerator.hpp"
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <fstream>
#include <sstream>
#include <stdexcept>

KeyGenerator::KeyGenerator(std::shared_ptr<RSAEncryptor> rsaEncryptor)
    : rsaEncryptor(rsaEncryptor)
{

    if (RAND_status() == 0)
    {
        throw std::runtime_error("No se pudo inicializar el generador de números aleatorios");
    }
}

std::vector<unsigned char> KeyGenerator::generateSecureEntropy(size_t bytes)
{
    std::vector<unsigned char> entropy(bytes);
    if (RAND_bytes(entropy.data(), bytes) != 1)
    {
        throw std::runtime_error("Error al generar entropía segura");
    }
    return entropy;
}

std::vector<unsigned char> KeyGenerator::generateAESKey(int keySize)
{

    if (keySize != 128 && keySize != 192 && keySize != 256)
    {
        throw std::runtime_error("Tamaño de clave AES no válido");
    }

    size_t keyLength = keySize / 8;
    std::vector<unsigned char> key(keyLength);

    if (RAND_bytes(key.data(), keyLength) != 1)
    {
        throw std::runtime_error("Error al generar clave AES");
    }

    return key;
}

std::string KeyGenerator::encryptKey(const std::vector<unsigned char> &aesKey)
{
    if (!rsaEncryptor)
    {
        throw std::runtime_error("Encriptador RSA no inicializado");
    }

    try
    {
        return rsaEncryptor->encrypt(aesKey);
    }
    catch (const RSAEncryptorException &e)
    {
        throw std::runtime_error(std::string("Error al cifrar clave: ") + e.what());
    }
}

std::vector<unsigned char> KeyGenerator::decryptKey(const std::string &encryptedKey)
{
    if (!rsaEncryptor)
    {
        throw std::runtime_error("Encriptador RSA no inicializado");
    }

    try
    {
        return rsaEncryptor->decrypt(encryptedKey);
    }
    catch (const RSAEncryptorException &e)
    {
        throw std::runtime_error(std::string("Error al descifrar clave: ") + e.what());
    }
}

std::string KeyGenerator::generateAndEncryptKey(int keySize)
{
    auto aesKey = generateAESKey(keySize);
    return encryptKey(aesKey);
}

bool KeyGenerator::storeKeyForFile(const std::string &filePath, const std::string &encryptedKey)
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    try
    {

        keyCache[filePath] = encryptedKey;

        std::ofstream keyFile(filePath + ".key");
        if (!keyFile)
            return false;

        keyFile << encryptedKey;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::string KeyGenerator::retrieveKeyForFile(const std::string &filePath)
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    auto it = keyCache.find(filePath);
    if (it != keyCache.end())
    {
        return it->second;
    }

    std::ifstream keyFile(filePath + ".key");
    if (!keyFile)
    {
        throw std::runtime_error("No se encontró clave para el archivo");
    }

    std::stringstream buffer;
    buffer << keyFile.rdbuf();
    std::string encryptedKey = buffer.str();

    keyCache[filePath] = encryptedKey;

    return encryptedKey;
}