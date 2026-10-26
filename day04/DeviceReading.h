#pragma once
#include <string>

struct DeviceReading {
    std::string deviceId;
    double temperature;
    bool online;
};