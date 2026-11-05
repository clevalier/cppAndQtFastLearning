#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "DeviceReading.h"

class MockCollector;

QT_BEGIN_NAMESPACE
class QTimer;
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
    void startCollection();
    void stopCollection();
    void onSamplingTimeout();
    void onReadingReady(const DeviceReading& reading);

private:
    MockCollector* collector_ = nullptr;
    bool alarmActive_ = false;
    QTimer* samplingTimer_ = nullptr;
    double t004Temperature_ = 45.8;
    void appendLog(const QString& message);

private:
    void saveSampleToJson();
    void loadSampleFromJson();

private:
    Ui::MainWindow *ui;
    void updateTemperatureById(
        const QString& deviceId,
        double temperature
        );
    void setCollecting(bool collecting);
};
#endif // MAINWINDOW_H
