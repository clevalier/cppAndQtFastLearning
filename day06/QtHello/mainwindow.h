#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


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

private:
    QTimer* samplingTimer_ = nullptr;
    double t004Temperature_ = 45.8;

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
