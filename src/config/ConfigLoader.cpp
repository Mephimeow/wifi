#include "ConfigLoader.h"
#include "../utils/Logger.h"
#include <fstream>
#include <sstream>

namespace {
    std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    std::string extractValue(const std::string& line) {
        size_t colon = line.find(':');
        if (colon == std::string::npos) return "";
        return trim(line.substr(colon + 1));
    }
}

AppConfig ConfigLoader::load(const std::string& path) {
    AppConfig config;
    std::ifstream file(path);
    
    if (!file.is_open()) {
        Logger::instance().warn("Config file not found: " + path + ", using defaults");
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        
        if (line.find("interface:") == 0) {
            config.interface = extractValue(line);
        } else if (line.find("adapter:") == 0) {
            config.adapterType = extractValue(line);
        } else if (line.find("timeout:") == 0) {
            try {
                config.scanTimeout = std::stoi(extractValue(line));
            } catch (...) {}
        }
    }

    Logger::instance().info("Loaded config: interface=" + config.interface + 
                          ", adapter=" + config.adapterType);
    return config;
}

bool ConfigLoader::save(const AppConfig& config, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        Logger::instance().error("Cannot save config: " + path);
        return false;
    }
    
    file << "interface: " << config.interface << "\n"
         << "adapter: " << config.adapterType << "\n"
         << "timeout: " << config.scanTimeout << "\n";
    
    return true;
}
