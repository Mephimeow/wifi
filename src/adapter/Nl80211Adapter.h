#pragma once
#include "IWiFiAdapter.h"
#include <string>

class Nl80211Adapter : public IWiFiAdapter {
private:
    std::string iface;

public:
    explicit Nl80211Adapter(const std::string& iface);

    std::vector<WiFiNetwork> scan() override;
    bool isAvailable() override;
};