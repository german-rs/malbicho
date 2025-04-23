#ifndef AES_ENCRYPTOR_HPP
#define AES_ENCRYPTOR_HPP

#include <vector>
#include <string>
#include <memory>
#include <openssl/evp.h>

class AESEncryptor
{
private:
    EVP_CIPHER_CTX *ctx;
    std::vector<std::string> targetExtensions;
    std::string encryptionAlgorithm;
    bool keySet;
    bool checkFileExtension(const std::string &filePath);
    void renameFile(const std::string &originalPath);

public:
    AESEncryptor();
    ~AESEncryptor();

    void generateRandomKey();
    bool encryptFile(const std::string &filePath);

    void setKey(const std::vector<unsigned char> &key,
                const std::vector<unsigned char> &iv);
    bool decryptFile(const std::vector<unsigned char> &ciphertext);

    void setTargetExtensions(const std::vector<std::string> &extensions);
    void setEncryptionAlgorithm(const std::string &algorithm);
};

#endif // AES_ENCRYPTOR_HPP