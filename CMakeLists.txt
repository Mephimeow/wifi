cmake_minimum_required(VERSION 3.10)
project(wifi_scanner)

set(CMAKE_CXX_STANDARD 17)

include_directories(src src/core src/adapter src/utils src/model)

add_executable(wifi_scanner
    src/app/main.cpp
    src/core/WifiScanner.cpp
    src/adapter/Nl80211Adapter.cpp
    src/adapter/SystemIWLAdapter.cpp
    src/utils/Logger.cpp
)