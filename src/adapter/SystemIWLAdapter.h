#pragma once
#include "IWiFiAdapter.h"
#include <string>

class SystemIWLAdapter : public IWiFiAdapter {
private:
    std::string iface;

public:
    explicit SystemIWLAdapter(const std::string& iface = "wlan0");
    std::vector<WiFiNetwork> scan() override;
    bool isAvailable() override;
};
