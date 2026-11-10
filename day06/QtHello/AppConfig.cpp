
#include "AppConfig.h"

#include <QSettings>
#include <QDebug>
#include <cmath>

AppConfig AppConfig::load()
{
    AppConfig config;

    QSettings settings(
        QSettings::IniFormat,
        QSettings::UserScope,
        "HardwareStudy",
        "QtHello"
        );

    // 读取报警阈值
    bool triggerOk = false;
    bool recoverOk = false;

    double trigger = settings.value(
                                 "alarm/trigger",
                                 config.alarmTrigger
                                 ).toDouble(&triggerOk);

    double recover = settings.value(
                                 "alarm/recover",
                                 config.alarmRecover
                                 ).toDouble(&recoverOk);

    if (triggerOk && recoverOk &&
        std::isfinite(trigger) &&
        std::isfinite(recover) &&
        recover < trigger)
    {
        config.alarmTrigger = trigger;
        config.alarmRecover = recover;
    }
    else
    {
        qWarning() << "报警配置非法，使用默认值";
    }

    // 读取 TCP 配置
    QString host = settings.value(
                               "tcp/host",
                               config.tcpHost
                               ).toString().trimmed();

    bool portOk = false;
    int port = settings.value(
                           "tcp/port",
                           config.tcpPort
                           ).toInt(&portOk);

    if (!host.isEmpty())
        config.tcpHost = host;

    if (portOk && port >= 1 && port <= 65535)
        config.tcpPort = static_cast<quint16>(port);

    if (settings.status() != QSettings::NoError)
        qWarning() << "配置文件读取异常";

    qDebug() << "配置路径：" << settings.fileName();
    qDebug() << "报警触发阈值：" << config.alarmTrigger;
    qDebug() << "报警恢复阈值：" << config.alarmRecover;
    qDebug() << "TCP 地址：" << config.tcpHost;
    qDebug() << "TCP 端口：" << config.tcpPort;

    return config;
}
