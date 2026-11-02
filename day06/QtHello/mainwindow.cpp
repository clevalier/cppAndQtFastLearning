#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DeviceReading.h"

#include <QHeaderView>
#include <QStatusBar>
#include <QTableWidgetItem>
#include <array>
#include <QPushButton>
#include <QStatusBar>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    samplingTimer_ = new QTimer(this);

    connect(
        samplingTimer_,
        &QTimer::timeout,
        this,
        &MainWindow::onSamplingTimeout
        );

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

    // updateTemperatureById("T004", 45.8);
    updateTemperatureById("T004", t004Temperature_);

    statusBar()->showMessage("系统就绪");
}


void MainWindow::loadSampleFromJson()
{
    QFile file("t004_sample.json");

    // 1. 打开文件
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "读取文件失败：" << file.errorString();
        return;
    }

    // 2. 读取全部字节
    QByteArray data = file.readAll();
    file.close();

    // 3. 解析 JSON
    QJsonParseError parseError;
    QJsonDocument doc =
        QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError ||
        !doc.isObject())
    {
        qWarning() << "JSON 解析失败："
                   << parseError.errorString();
        return;
    }

    // 4. 获取字段
    QJsonObject obj = doc.object();

    if (!obj.value("deviceId").isString() ||
        !obj.value("temperature").isDouble() ||
        !obj.value("timestamp").isString())
    {
        qWarning() << "JSON 字段缺失或类型错误";
        return;
    }

    qDebug() << "设备：" << obj.value("deviceId").toString();
    qDebug() << "温度：" << obj.value("temperature").toDouble();
    qDebug() << "时间：" << obj.value("timestamp").toString();
}


void MainWindow::saveSampleToJson()
{
    // 1. 构造 JSON 对象
    QJsonObject obj;
    obj["deviceId"] = "T004";
    obj["temperature"] = t004Temperature_;
    obj["timestamp"] =
        QDateTime::currentDateTime()
            .toString(Qt::ISODateWithMs);

    // 2. 将 JSON 对象转成 UTF-8 字节
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Indented);

    // 3. 打开文件
    QFile file("t004_sample.json");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qWarning() << "文件打开失败：" << file.errorString();
        return;
    }

    // 4. 写入数据
    if (file.write(data) != data.size())
    {
        qWarning() << "文件写入失败：" << file.errorString();
        return;
    }

    file.close();

    qDebug() << "JSON 保存成功，路径："
             << QDir::current().absoluteFilePath(
                    "t004_sample.json");
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
    // 每 1000 毫秒触发一次
    samplingTimer_->start(1000);
}

void MainWindow::stopCollection()
{
    // 停止定时更新
    samplingTimer_->stop();
    setCollecting(false);

    // 保存最后一次采样值
    saveSampleToJson();

    // 重新读取验证
    loadSampleFromJson();
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


void MainWindow::onSamplingTimeout()
{
    // 模拟设备温度每次增加 0.1℃
    t004Temperature_ += 0.1;

    // 只更新 T004 对应的温度单元格
    updateTemperatureById(
        "T004",
        t004Temperature_
        );

    // 在状态栏显示最新温度
    statusBar()->showMessage(
        QString("T004 当前温度：%1 ℃")
            .arg(t004Temperature_, 0, 'f', 1)
        );
}



MainWindow::~MainWindow()
{
    delete ui;
}
