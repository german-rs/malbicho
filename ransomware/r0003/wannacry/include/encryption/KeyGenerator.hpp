// KeyGenerator.hpp (Actualizado según relaciones del UML)
#ifndef KEY_GENERATOR_HPP
#define KEY_GENERATOR_HPP

#include <string>
#include <vector>
#include "../core/Constants.hpp"

class KeyGenerator
{
public:
    // Métodos necesarios para soportar FileEncryptor del UML
    static std::string generateRandomKey(int keySize = Constants::Encryption::RSA_KEY_SIZE);

    // Métodos para generación de claves específicas
    static std::vector<unsigned char> generateSecureIV();
    static std::string generateFileSpecificKey(const std::string &baseKey,
                                               const std::string &filePath);

    // Métodos para integración con RSA
    static std::pair<std::string, std::string> generateRSAKeyPair(int keySize);

    // Métodos para validación de claves
    static bool validateAESKey(const std::vector<unsigned char> &key);
    static bool validateRSAKey(const std::string &key);
};

#endif // KEY_GENERATOR_HPP