#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "DeviceReading.h"
#include <QByteArray>
#include "AlarmManager.h"
#include "DeviceStore.h"

class MockCollector;

QT_BEGIN_NAMESPACE

class QTcpSocket;
class QTimer;
class QThread;
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;


private slots:
     void startSlowWorker();
    void startCollection();
    void stopCollection();
    void onSamplingTimeout();
    void onReadingReady(const DeviceReading& reading);
    void onTcpReadyRead();


private:
    DeviceStore deviceStore_;
    void applyReading(const DeviceReading& reading);
    AlarmManager alarmManager_;
    void handleAlarm(const DeviceReading& reading);
    QThread* slowThread_ = nullptr;
    MockCollector* collector_ = nullptr;
    // bool alarmActive_ = false;
    QTimer* samplingTimer_ = nullptr;
    double t004Temperature_ = 45.8;
    void appendLog(const QString& message);
    QTcpSocket* tcpSocket_ = nullptr;
    QByteArray tcpBuffer_;

private:
    void saveSampleToJson();
    void loadSampleFromJson();
    void updateDeviceReading(const DeviceReading& reading);
private:
    Ui::MainWindow *ui;
    void updateTemperatureById(
        const QString& deviceId,
        double temperature
        );
    void setCollecting(bool collecting);
};
#endif // MAINWINDOW_H
