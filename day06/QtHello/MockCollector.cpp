
#include "MockCollector.h"
#include <QTimer>

MockCollector::MockCollector(QObject* parent)
    : QObject(parent)
{
    timer_ = new QTimer(this);

    connect(
        timer_,
        &QTimer::timeout,
        this,
        &MockCollector::generateSample
        );
}

void MockCollector::start()
{
    timer_->start(1000);
}

void MockCollector::stop()
{
    timer_->stop();
}

const DeviceReading& MockCollector::currentReading() const
{
    return t004_;
}

void MockCollector::generateSample()
{
    // 模拟每次温度增加 0.1℃
    t004_.temperature += 0.1;

    // 发布新的设备数据
    emit readingReady(t004_);
}
