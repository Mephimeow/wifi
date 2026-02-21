#pragma once
#include <string>

struct WiFiNetwork {
    std::string ssid;
    std::string bssid;
    int signal = 0; // dBm
    int frequency = 0;
};