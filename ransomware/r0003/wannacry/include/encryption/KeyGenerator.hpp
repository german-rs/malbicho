class KeyGenerator
{
private:
    // Generador de números aleatorios criptográficamente seguros
    std::unique_ptr<RandomNumberGenerator> rng;

    // Referencia al encriptador RSA para cifrar las claves AES
    std::shared_ptr<RSAEncryptor> rsaEncryptor;

    // Caché de claves (para uso interno)
    std::map<std::string, std::string> keyCache;

    // Método para generar entropía segura
    std::vector<unsigned char> generateSecureEntropy(size_t bytes);

public:
    // Constructor que recibe un encriptador RSA inicializado
    KeyGenerator(std::shared_ptr<RSAEncryptor> rsaEncryptor);

    // Genera una nueva clave AES aleatoria
    std::vector<unsigned char> generateAESKey(int keySize = 256);

    // Cifra una clave AES con RSA
    std::string encryptKey(const std::vector<unsigned char> &aesKey);

    // Descifra una clave AES (requiere clave privada RSA)
    std::vector<unsigned char> decryptKey(const std::string &encryptedKey);

    // Genera y cifra una clave en una sola operación
    std::string generateAndEncryptKey(int keySize = 256);

    // Almacena información de clave para un archivo específico
    bool storeKeyForFile(const std::string &filePath, const std::string &encryptedKey);

    // Recupera la clave cifrada para un archivo
    std::string retrieveKeyForFile(const std::string &filePath);
};