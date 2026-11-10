
#include "AlarmManager.h"

AlarmManager::AlarmManager(
    double triggerThreshold,
    double recoverThreshold)
    : triggerThreshold_(triggerThreshold),
    recoverThreshold_(recoverThreshold)
{
}

AlarmManager::Event AlarmManager::evaluate(
    const QString& deviceId,
    double temperature)
{
    // 查询当前设备是否已经处于报警状态
    bool active = active_.value(deviceId, false);

    // 原先正常，超过阈值：触发报警
    if (!active && temperature > triggerThreshold_)
    {
        active_[deviceId] = true;
        return Event::Triggered;
    }

    // 原先报警，温度降至恢复阈值：恢复
    if (active && temperature <= recoverThreshold_)
    {
        active_[deviceId] = false;
        return Event::Recovered;
    }

    // 报警状态没有发生变化
    return Event::None;
}
