#include "config.hpp"
#include "ini.h"
#include <stdexcept>
#include <iostream>

static int config_ini_handler(void* user, const char* section, const char* name, const char* value) {
    Config* config = static_cast<Config*>(user);
    if (std::string(section) == "monitor") {
        if (std::string(name) == "interface") {
            config->interface = value;
        } else if (std::string(name) == "update_interval_ms") {
            try {
                config->update_interval_ms = std::stoi(value);
                if (config->update_interval_ms <= 0) {
                    throw std::invalid_argument("update_interval_ms must be positive");
                }
            } catch (const std::exception& e) {
                std::cerr << "Invalid update_interval_ms: " << e.what() << std::endl;
            }
        }
    }
    return 1;
}

Config load_config(const std::string& filename) {
    Config config;
    if (ini_parse(filename.c_str(), config_ini_handler, &config) < 0) {
        std::cerr << "Failed to load config file: " << filename << ", using defaults" << std::endl;
    }
    return config;
}