#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "IWiFiAdapter.h"

struct nl_msg;
struct nl_sock;

class Nl80211Adapter : public IWiFiAdapter {
public:
    explicit Nl80211Adapter(const std::string& iface);
    ~Nl80211Adapter();

    std::vector<WiFiNetwork> scan() override;
    bool isAvailable() override;

private:
    bool initSocket();
    void cleanup();
    int sendScanCommand();
    int recvScanResults();
    static int callbackHandler(struct nl_msg* msg, void* arg);

    std::string iface;
    int ifindex = -1;
    nl_sock* sock = nullptr;
    int familyId = -1;
    std::vector<WiFiNetwork> results;
};
