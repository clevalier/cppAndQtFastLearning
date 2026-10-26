#pragma once

#include "DeviceReading.h"
#include <vector>
#include <map>
#include <string>

std::vector<DeviceReading> filterAlarms(
    const std::vector<DeviceReading>& readings,
    double threshold
);

double averageOnlineTemperature(
    const std::vector<DeviceReading>& readings
);

std::map<std::string, double> buildTemperatureIndex(
    const std::vector<DeviceReading>& readings
);