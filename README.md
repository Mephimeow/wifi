wifi-scanner/
│
├── CMakeLists.txt
├── config/
│   └── config.yaml
└── src/
    ├── app/
    │   └── main.cpp                    #точка входа
    ├── core/
    │   ├── WiFiScanner.h               #обьединение адаптеров и логики
    │   └── WiFiScanner.cpp
    ├── adapter/
    │   ├── IWiFiAdapter.h              #чистый интерфейс реализующий все типы сканеров
    │   ├── Nl80211Adapter.cpp          
    │   ├── Nl80211Adapter.h            #адаптер работающий через Linux nl80211/netlink API
    │   ├── SystemIWLAdapter.cpp        
    │   └── SystemIWLAdapter.h          #адаптер без libnl и root
    ├── utils/
    │   ├── Logger.cpp
    │   └── Logger.h                    #логгер , если кто не понял
    └── models/
        └── WiFiNetwork.h               #структура сети
