
#pragma once

#include <QObject>
#include "DeviceReading.h"

class QTimer;

class MockCollector : public QObject
{
    Q_OBJECT

public:
    explicit MockCollector(QObject* parent = nullptr);

    void start();
    void stop();

    const DeviceReading& currentReading() const;

signals:
    void readingReady(const DeviceReading& reading);

private:
    void generateSample();

    QTimer* timer_ = nullptr;

    DeviceReading t004_{
        "T004", 45.8, true, 0.85
    };
};
