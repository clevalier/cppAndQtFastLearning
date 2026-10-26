
#include "Analyzer.h"

#include <algorithm>
#include <iterator>

std::vector<DeviceReading> filterAlarms(
    //引用已有容器，只读取
    const std::vector<DeviceReading>& readings,
    //报警阈值，例如 30.0
    double threshold 
) {
    //创建一个空的 vector，专门保存筛选出来的报警记录。
    std::vector<DeviceReading> result;
    //std::copy_if() 就是遍历一个容器，判断每个元素是否满足条件，满足就复制到目标容器。
    //1.告诉算法从哪里开始遍历，指向容器的第一个元素。
    //2.告诉算法遍历到哪里结束。它表示最后一个元素之后的位置，不是最后一个元素本身。
    //3.std::back_inserter(result)告诉算法把符合条件的记录追加到 result 末尾，内部会使用 push_back()
    //4.最后一个参数是一个 Lambda 表达式，表示筛选条件。它接收一个 DeviceReading 对象，返回 true 或 false。
    std::copy_if(
        readings.begin(),
        readings.end(),
        std::back_inserter(result),
        [threshold](const DeviceReading& item) {
            return item.online &&
                   item.temperature > threshold;
        }
    );

    return result;
}

double averageOnlineTemperature(
    const std::vector<DeviceReading>& readings
) {
    double total = 0.0;
    int count = 0;

    for (const auto& item : readings) {
        if (item.online) {
            total += item.temperature;
            count++;
        }
    }

    return count > 0 ? total / count : 0.0;
}

std::map<std::string, double> buildTemperatureIndex(
    const std::vector<DeviceReading>& readings
) {
    std::map<std::string, double> index;

    for (const auto& item : readings) {
        index[item.deviceId] = item.temperature;
    }

    return index;
}
