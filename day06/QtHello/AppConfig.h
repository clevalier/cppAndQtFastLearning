#pragma once

#include <QString>
#include <QtGlobal>

struct AppConfig
{
    double alarmTrigger = 46.0;
    double alarmRecover = 45.0;

    QString tcpHost = "127.0.0.1";
    quint16 tcpPort = 9000;

    static AppConfig load();
};