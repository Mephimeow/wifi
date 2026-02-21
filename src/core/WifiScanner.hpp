#pragma once
#include <memory>
#include "../adapter/IWiFiAdapter.h"

class WiFiScanner {
private:
    std::unique_ptr<IWiFiAdapter> adapter;

public:
    explicit WiFiScanner(std::unique_ptr<IWiFiAdapter> adapter);

    std::vector<WiFiNetwork> scan();
};