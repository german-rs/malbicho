/**
 * @file KeyGenerator.hpp
 * @brief Clase para la generación y validación de claves criptográficas.
 *
 * Esta clase proporciona funcionalidades para generar varios tipos de claves
 * criptográficas, incluyendo claves RSA, IVs seguros y claves específicas para archivos.
 * También incluye métodos para validar la integridad de las claves generadas.
 */

#ifndef KEY_GENERATOR_HPP
#define KEY_GENERATOR_HPP

#include <string>
#include <vector>
#include "../core/Constants.hpp"

/**
 * @class KeyGenerator
 * @brief Clase utilitaria para generar y validar claves criptográficas.
 *
 * Proporciona métodos estáticos para la generación de diferentes tipos de claves
 * para su uso en operaciones criptográficas. Incluye soporte para claves AES y RSA,
 * así como para la validación de claves.
 */
class KeyGenerator
{
public:
    /**
     * @brief Genera una clave aleatoria.
     *
     * @param keySize Tamaño de la clave a generar (bits).
     * @return std::string La clave generada.
     */
    static std::string generateRandomKey(int keySize = Constants::Encryption::RSA_KEY_SIZE);

    /**
     * @brief Genera un Vector de Inicialización (IV) seguro para algoritmos de cifrado.
     *
     * @return std::vector<unsigned char> El IV generado como vector de bytes.
     */
    static std::vector<unsigned char> generateSecureIV();

    /**
     * @brief Genera una clave específica para un archivo basada en una clave base.
     *
     * @param baseKey La clave base que se usará para derivar la clave específica.
     * @param filePath Ruta del archivo para el que se generará la clave específica.
     * @return std::string La clave específica para el archivo.
     */
    static std::string generateFileSpecificKey(const std::string &baseKey,
                                               const std::string &filePath);

    /**
     * @brief Genera un par de claves RSA (pública y privada).
     *
     * @param keySize Tamaño de las claves RSA a generar (bits).
     * @return std::pair<std::string, std::string> Par de claves generadas (pública, privada).
     */
    static std::pair<std::string, std::string> generateRSAKeyPair(int keySize);

    /**
     * @brief Valida una clave AES.
     *
     * @param key La clave AES a validar.
     * @return bool true si la clave es válida, false en caso contrario.
     */
    static bool validateAESKey(const std::vector<unsigned char> &key);

    /**
     * @brief Valida una clave RSA.
     *
     * @param key La clave RSA a validar.
     * @return bool true si la clave es válida, false en caso contrario.
     */
    static bool validateRSAKey(const std::string &key);
};

#endif // KEY_GENERATOR_HPP