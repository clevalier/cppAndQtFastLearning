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

    statusBar()->showMessage("系统就绪");
}

MainWindow::~MainWindow()
{
    delete ui;
}
