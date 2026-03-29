#include "Nl80211Adapter.h"
#include "../utils/Logger.h"
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <linux/nl80211.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <cstring>

Nl80211Adapter::Nl80211Adapter(const std::string& iface) : iface(iface) {
    ifindex = static_cast<int>(if_nametoindex(iface.c_str()));
}

Nl80211Adapter::~Nl80211Adapter() {
    cleanup();
}

bool Nl80211Adapter::isAvailable() {
    if (ifindex == 0) return false;
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return false;
    
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface.c_str(), IFNAMSIZ - 1);
    
    bool available = (::ioctl(sock, SIOCGIFFLAGS, &ifr) == 0);
    close(sock);
    return available;
}

bool Nl80211Adapter::initSocket() {
    sock = nl_socket_alloc();
    if (!sock) {
        Logger::instance().error("Failed to allocate nl socket");
        return false;
    }

    if (genl_connect(sock) < 0) {
        Logger::instance().error("Failed to connect to generic netlink");
        nl_socket_free(sock);
        sock = nullptr;
        return false;
    }

    familyId = genl_ctrl_resolve(sock, NL80211_GENL_NAME);
    if (familyId < 0) {
        Logger::instance().error("NL80211 not found - is cfg80211 loaded?");
        nl_socket_free(sock);
        sock = nullptr;
        return false;
    }

    nl_socket_set_nonblocking(sock);
    return true;
}

void Nl80211Adapter::cleanup() {
    if (sock) {
        nl_socket_free(sock);
        sock = nullptr;
    }
}

int Nl80211Adapter::sendScanCommand() {
    nl_msg* msg = nlmsg_alloc();
    if (!msg) {
        Logger::instance().error("Failed to allocate nlmsg");
        return -1;
    }

    auto hdr = genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, familyId, 0,
                            NLM_F_DUMP, NL80211_CMD_TRIGGER_SCAN, 0);
    if (!hdr) {
        Logger::instance().error("Failed to put nlmsg header");
        nlmsg_free(msg);
        return -1;
    }

    nla_put_u32(msg, NL80211_ATTR_IFINDEX, ifindex);
    
    int ret = nl_send_auto(sock, msg);
    nlmsg_free(msg);
    
    if (ret < 0) {
        Logger::instance().error("Failed to send scan command: " + std::to_string(ret));
        return ret;
    }

    return 0;
}

std::vector<WiFiNetwork> Nl80211Adapter::scan() {
    results.clear();

    if (!initSocket()) {
        return results;
    }

    if (sendScanCommand() < 0) {
        cleanup();
        return results;
    }

    Logger::instance().info("Scan triggered, waiting for results...");

    int ret;
    int waited = 0;
    while (waited < 10000) {
        ret = nl_recvmsgs_default(sock);
        if (ret < 0) {
            if (ret == -NLE_INTR) {
                usleep(100000);
                waited += 100;
                continue;
            }
            break;
        }
        waited += 100;
        usleep(100000);
    }

    cleanup();
    
    if (results.empty()) {
        Logger::instance().warn("No networks found (may need root)");
    }
    
    return results;
}
