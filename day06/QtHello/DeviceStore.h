
#pragma once

#include <QHash>
#include <QString>
#include "DeviceReading.h"

class DeviceStore
{
public:
    void update(const DeviceReading& reading);

    bool get(
        const QString& deviceId,
        DeviceReading& result
        ) const;

    int count() const;

private:
    QHash<QString, DeviceReading> readings_;
};
