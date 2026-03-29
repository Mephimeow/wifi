#include "SystemIWLAdapter.h"
#include "../utils/Logger.h"
#include <fstream>
#include <sstream>
#include <iomanip>

SystemIWLAdapter::SystemIWLAdapter(const std::string& iface) : iface(iface) {}

bool SystemIWLAdapter::isAvailable() {
    std::ifstream file("/proc/net/wireless");
    return file.good();
}

std::vector<WiFiNetwork> SystemIWLAdapter::scan() {
    std::vector<WiFiNetwork> networks;
    
    std::ifstream file("/proc/net/wireless");
    if (!file.is_open()) {
        Logger::instance().error("Cannot open /proc/net/wireless");
        return networks;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find(iface) != std::string::npos) {
            std::istringstream iss(line);
            std::string ifaceName, status, link, level, noise;
            
            iss >> ifaceName >> status >> link >> level >> noise;
            
            WiFiNetwork net;
            net.ssid = iface;
            net.bssid = "00:00:00:00:00:00";
            
            try {
                net.signal = static_cast<int>((std::stof(link) / 2.0) - 100);
            } catch (...) {
                net.signal = -100;
            }
            
            networks.push_back(net);
            break;
        }
    }

    return networks;
}
