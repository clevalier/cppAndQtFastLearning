#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
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

private:
    Ui::MainWindow *ui;
    void updateTemperatureById(
        const QString& deviceId,
        double temperature
        );
    void setCollecting(bool collecting);
};
#endif // MAINWINDOW_H
