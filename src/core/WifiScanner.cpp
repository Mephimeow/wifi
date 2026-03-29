#include "WifiScanner.hpp"

WiFiScanner::WiFiScanner(std::unique_ptr<IWiFiAdapter> adapter) 
    : adapter(std::move(adapter)) {}

std::vector<WiFiNetwork> WiFiScanner::scan() {
    return adapter->scan();
}
