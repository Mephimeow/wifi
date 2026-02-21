#pragma once
#include <vector>
#include "../model/WiFiNetwork.hpp"

class IWiFiAdapter {
public:
    virtual ~IWiFiAdapter() = default;
    virtual std::vector<WiFiNetwork> scan() = 0;
    virtual bool isAvailable() { return true; }
};