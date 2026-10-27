#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DeviceReading.h"

#include <QHeaderView>
#include <QStatusBar>
#include <QTableWidgetItem>
#include <array>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 用结构体保存练习数据，再逐行填入 Designer 创建的表格。
    const std::array<DeviceReading, 4> readings{{
        {"T001", 26.5, true},
        {"T002", 31.2, true},
        {"T003", 28.0, false},
        {"T004", 35.8, true}
    }};

    auto *table = ui->tableWidget;
    table->setRowCount(static_cast<int>(readings.size()));
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"设备编号", "温度", "状态"});
    for (int row = 0; row < table->rowCount(); ++row) {
        const auto &reading = readings[row];
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(reading.deviceId)));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(reading.temperature, 'f', 1)));
        table->setItem(row, 2, new QTableWidgetItem(reading.online ? "在线" : "离线"));
    }
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    updateTemperatureById("T004", 45.8);

    statusBar()->showMessage("系统就绪");
}


void MainWindow::updateTemperatureById(
    const QString& deviceId,
    double temperature)
{
    auto* table = ui->tableWidget;

    // 遍历表格每一行
    for (int row = 0; row < table->rowCount(); ++row)
    {
        // 获取当前行的设备编号
        auto* idItem = table->item(row, 0);

        if (idItem == nullptr)
        {
            continue;
        }

        // 判断是不是要更新的设备
        if (idItem->text() == deviceId)
        {
            // 获取温度单元格
            auto* tempItem = table->item(row, 1);

            if (tempItem != nullptr)
            {
                tempItem->setText(
                    QString::number(temperature, 'f', 1)
                    );
            }

            return;
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
