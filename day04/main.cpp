
#include "Analyzer.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    std::vector<DeviceReading> readings = {
        {"T001", 26.5, true},
        {"T002", 31.2, true},
        {"T003", 28.0, false},
        {"T004", 35.8, true},
        {"T005", 29.1, true}
    };

    const double threshold = 30.0;

    auto alarms = filterAlarms(readings, threshold);
    auto index = buildTemperatureIndex(readings);

    auto onlineCount = std::count_if(
        readings.begin(),
        readings.end(),
        [](const DeviceReading& item) {
            return item.online;
        }
    );

    std::cout << std::fixed
              << std::setprecision(2);

    std::cout << "采样记录数："
              << readings.size() << '\n';

    std::cout << "在线设备数："
              << onlineCount << '\n';

    std::cout << "在线平均温度："
              << averageOnlineTemperature(readings)
              << " C\n";

    std::cout << "报警设备数："
              << alarms.size() << '\n';

    for (const auto& item : alarms) {
        std::cout << "报警："
                  << item.deviceId << " "
                  << item.temperature << " C\n";
    }

    std::cout << "T002 温度："
              << index.at("T002") << " C\n";

    return 0;
}
