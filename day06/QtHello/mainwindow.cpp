#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DeviceReading.h"

#include <QHeaderView>
#include <QStatusBar>
#include <QTableWidgetItem>
#include <array>
#include <QPushButton>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化按钮状态
    // ui->startButton->setEnabled(true);
    // ui->stopButton->setEnabled(false);
    setCollecting(false);

    //开始采集
    connect(
        ui->startButton,//信号发送者。
        &QPushButton::clicked,//要监听的信号 QPushButton代表类名，::代表作用域解析运算符 QPushButton代表定义的信号 &取得成员函数指针
        this,//接收上下文对象
        // [this]() {//信号触发时执行的函数
        //     ui->statusLabel->setText("设备状态：采集中");
        //     statusBar()->showMessage("正在模拟采集");
        //     ui->startButton->setEnabled(false);
        //     ui->stopButton->setEnabled(true);
        // }
        &MainWindow::startCollection
    );
    //停止采集
    connect(
        ui->stopButton,//信号发送者。
        &QPushButton::clicked,//要监听的信号 QPushButton代表类名，::代表作用域解析运算符 QPushButton代表定义的信号 &取得成员函数指针
        this,//接收上下文对象
        // [this]() {//信号触发时执行的函数
        //     ui->statusLabel->setText("设备状态：待机");
        //     statusBar()->showMessage("采集已停止 / 系统待机");
        //     ui->stopButton->setEnabled(false);
        //     ui->startButton->setEnabled(true);
        // }
        &MainWindow::stopCollection
    );

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

void MainWindow::startCollection()
{
    setCollecting(true);
}

void MainWindow::stopCollection()
{
    setCollecting(false);
}


void MainWindow::setCollecting(bool collecting)
{
    ui->statusLabel->setText(
        collecting
            ? "设备状态：采集中"
            : "设备状态：待机"
        );

    statusBar()->showMessage(
        collecting
            ? "正在模拟采集"
            : "采集已停止 / 系统待机"
        );

    ui->startButton->setEnabled(!collecting);
    ui->stopButton->setEnabled(collecting);
}


MainWindow::~MainWindow()
{
    delete ui;
}
