# WiFi Scanner

Сканер WiFi сетей для Linux.

## Сборка

```bash
mkdir build && cd build
cmake ..
make
```

Для nl80211 адаптера (полное сканирование):
```bash
sudo apt install libnl-3-dev libnl-genl-3-dev
```

## Запуск

```bash
./wifi_scanner
```

Или с sudo для nl80211:
```bash
sudo ./wifi_scanner
```

## Конфигурация

Файл `config/config.yaml`:

```yaml
interface: wlan0
adapter: system   # system или nl80211
timeout: 5000
```

system - Нет зависимостей (сигнал текущей сети)
nl80211 - root + libnl (полное сканирование всех сетей) 

## Структура

```
src/
├── app/main.cpp           # точка входа
├── core/WifiScanner.hpp   # обертка над адаптером
├── adapter/
│   ├── IWiFiAdapter.h     # интерфейс
│   ├── Nl80211Adapter.*   # nl80211/netlink
│   └── SystemIWLAdapter.* # /proc/net/wireless
├── config/
│   └── ConfigLoader.*     # загрузка config.yaml
├── model/WiFiNetwork.hpp  # структура сети
└── utils/Logger.*          # логирование
```
