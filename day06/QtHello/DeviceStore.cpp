
#include "DeviceStore.h"

void DeviceStore::update(
    const DeviceReading& reading)
{
    QString id =
        QString::fromStdString(reading.deviceId);

    // 不存在则插入，存在则更新
    readings_.insert(id, reading);
}

bool DeviceStore::get(
    const QString& deviceId,
    DeviceReading& result
    ) const
{
    auto it = readings_.constFind(deviceId);

    if (it == readings_.constEnd())
        return false;

    result = it.value();
    return true;
}

int DeviceStore::count() const
{
    return static_cast<int>(readings_.size());
}
