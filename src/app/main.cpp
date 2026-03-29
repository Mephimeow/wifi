#include "core/WifiScanner.hpp"
#include "adapter/SystemIWLAdapter.h"
#ifdef HAVE_LIBNL
#include "adapter/Nl80211Adapter.h"
#endif
#include "config/ConfigLoader.h"
#include "utils/Logger.h"
#include <iostream>
#include <memory>

void printNetworks(const std::vector<WiFiNetwork>& networks) {
    if (networks.empty()) {
        std::cout << "No networks found.\n";
        return;
    }

    std::cout << "\n+----------------------+-----------------+----------+--------+\n";
    std::cout << "| SSID                 | BSSID           | Signal   | Freq   |\n";
    std::cout << "+----------------------+-----------------+----------+--------+\n";
    
    for (const auto& net : networks) {
        std::string ssid = net.ssid.empty() ? "<hidden>" : net.ssid;
        if (ssid.length() > 20) ssid = ssid.substr(0, 17) + "...";
        
        printf("| %-20s | %-15s | %7d dBm | %5d |\n",
               ssid.c_str(), net.bssid.c_str(), net.signal, net.frequency);
    }
    std::cout << "+----------------------+-----------------+----------+--------+\n";
    std::cout << "Total: " << networks.size() << " network(s)\n";
}

int main(int argc, char* argv[]) {
    auto config = ConfigLoader::load();
    
    std::unique_ptr<IWiFiAdapter> adapter;
    
    if (config.adapterType == "nl80211") {
#ifdef HAVE_LIBNL
        Logger::instance().info("Using nl80211 adapter (requires root)");
        adapter = std::make_unique<Nl80211Adapter>(config.interface);
#else
        Logger::instance().warn("nl80211 adapter not available, using system adapter");
        adapter = std::make_unique<SystemIWLAdapter>(config.interface);
#endif
    } else {
        Logger::instance().info("Using system adapter (no root required)");
        adapter = std::make_unique<SystemIWLAdapter>(config.interface);
    }

    if (!adapter->isAvailable()) {
        Logger::instance().error("WiFi adapter '" + config.interface + "' not available");
        return 1;
    }

    Logger::instance().info("Starting scan on " + config.interface + "...");
    
    auto networks = adapter->scan();
    
    printNetworks(networks);
    
    Logger::instance().info("Scan complete");
    return 0;
}
