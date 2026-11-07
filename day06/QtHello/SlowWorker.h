
#pragma once

#include <QObject>

class SlowWorker : public QObject
{
    Q_OBJECT

public:
    explicit SlowWorker(QObject* parent = nullptr)
        : QObject(parent) {}

public slots:
    void doWork();

signals:
    void readingReady(double temperature);
    void finished();
};
