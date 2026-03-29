#include "core/WifiScanner.hpp"
#include "adapter/SystemIWLAdapter.h"
#include "utils/Logger.h"
#include <iostream>
#include <memory>

int main() {
    Logger::instance().info("WiFi Scanner started");
    
    auto adapter = std::make_unique<SystemIWLAdapter>("wlan0");
    
    if (!adapter->isAvailable()) {
        Logger::instance().error("WiFi adapter not available");
        return 1;
    }
    
    WiFiScanner scanner(std::move(adapter));
    
    auto networks = scanner.scan();
    
    if (networks.empty()) {
        Logger::instance().warn("No networks found");
        return 0;
    }
    
    for (const auto& net : networks) {
        std::cout << "SSID: " << net.ssid 
                  << " | BSSID: " << net.bssid 
                  << " | Signal: " << net.signal << " dBm"
                  << " | Freq: " << net.frequency << " MHz" << std::endl;
    }
    
    Logger::instance().info("Scan complete. Found " + std::to_string(networks.size()) + " network(s)");
    return 0;
}
