
#pragma once

#include <QString>
#include <QHash>

class AlarmManager
{
public:
    enum class Event
    {
        None,
        Triggered,
        Recovered
    };

    explicit AlarmManager(
        double triggerThreshold = 46.0,
        double recoverThreshold = 45.0
        );

    Event evaluate(
        const QString& deviceId,
        double temperature
        );

private:
    double triggerThreshold_;
    double recoverThreshold_;

    QHash<QString, bool> active_;
};
