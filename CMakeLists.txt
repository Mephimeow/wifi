cmake_minimum_required(VERSION 3.10)
project(wifi_scanner)

set(CMAKE_CXX_STANDARD 17)

include_directories(src src/core src/adapter src/utils src/model src/config)

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(LIBNL libnl-3.0 libnl-genl-3.0 QUIET)
endif()

add_executable(wifi_scanner
    src/app/main.cpp
    src/core/WifiScanner.cpp
    src/adapter/SystemIWLAdapter.cpp
    src/utils/Logger.cpp
    src/config/ConfigLoader.cpp
)

if(LIBNL_FOUND)
    target_include_directories(wifi_scanner PRIVATE ${LIBNL_INCLUDE_DIRS})
    target_link_libraries(wifi_scanner PRIVATE ${LIBNL_LIBRARIES})
    target_sources(wifi_scanner PRIVATE src/adapter/Nl80211Adapter.cpp)
    target_compile_definitions(wifi_scanner PRIVATE HAVE_LIBNL=1)
    message(STATUS "nl80211 adapter: enabled (libnl)")
else()
    message(STATUS "nl80211 adapter: disabled (libnl not found)")
    message(STATUS "  Install libnl: sudo apt install libnl-3-dev libnl-genl-3-dev")
endif()
