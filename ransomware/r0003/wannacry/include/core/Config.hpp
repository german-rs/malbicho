#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <fstream>
// #include <json/json.hpp>

/**
 * @class Config
 * @brief Gestiona la configuración global del ransomware.
 *
 * Esta clase implementa el patrón Singleton para garantizar una única instancia
 * de configuración en todo el sistema. Proporciona acceso a parámetros
 * de configuración, tanto predefinidos como los obtenidos dinámicamente.
 */

class Config
{
private:
    // Singleton pattern
    static std::shared_ptr<Config> instance;
    static std::mutex instanceMutex;

    // Datos de configuración
    std::map<std::string, std::string> stringConfig;
};

#endif // CONFIG_HPP