#pragma once
#include <string>
#include <memory>

struct AppConfig {
    std::string interface = "wlan0";
    std::string adapterType = "system"; // "system" or "nl80211"
    int scanTimeout = 5000;
};

class ConfigLoader {
public:
    static AppConfig load(const std::string& path = "config/config.yaml");
    static bool save(const AppConfig& config, const std::string& path = "config/config.yaml");
};
