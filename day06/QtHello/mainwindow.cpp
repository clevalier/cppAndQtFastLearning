#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DeviceReading.h"

#include <QHeaderView>
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
#include <QPlainTextEdit>
#include "MockCollector.h"
#include <QTcpSocket>
#include <QAbstractSocket>
#include "SlowWorker.h"
#include <QThread>
#include "AlarmManager.h"
#include <QSettings>
#include "AppConfig.h"
#include "DeviceStore.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // QSettings settings(
    //     QSettings::IniFormat,
    //     QSettings::UserScope,
    //     "HardwareStudy",
    //     "QtHello"
    //     );

    // // 首次运行时创建默认配置
    // if (!settings.contains("alarm/trigger"))
    //     settings.setValue("alarm/trigger", 46.0);

    // if (!settings.contains("alarm/recover"))
    //     settings.setValue("alarm/recover", 45.0);

    // if (!settings.contains("tcp/host"))
    //     settings.setValue("tcp/host", "127.0.0.1");

    // if (!settings.contains("tcp/port"))
    //     settings.setValue("tcp/port", 9000);

    // settings.sync();

    // if (settings.status() != QSettings::NoError)
    // {
    //     qWarning() << "配置文件访问失败";
    // }

    // // 读取报警阈值
    // double trigger =
    //     settings.value("alarm/trigger", 46.0).toDouble();

    // double recover =
    //     settings.value("alarm/recover", 45.0).toDouble();

    // // 避免配置错误导致报警逻辑异常
    // if (recover >= trigger)
    // {
    //     qWarning() << "报警阈值配置不合法，使用默认值";
    //     trigger = 46.0;
    //     recover = 45.0;
    // }
    const AppConfig config = AppConfig::load();

    alarmManager_ = AlarmManager(
        config.alarmTrigger,
        config.alarmRecover
        );

    // 将配置交给 AlarmManager
    // alarmManager_ = AlarmManager(trigger, recover);

    // qDebug() << "配置路径：" << settings.fileName();
    // qDebug() << "报警触发阈值：" << trigger;
    // qDebug() << "报警恢复阈值：" << recover;

    ui->logEdit->setReadOnly(true);
    ui->logEdit->setMaximumBlockCount(300);
    //创建采集器
    collector_ = new MockCollector(this);

    connect(
        ui->threadTestButton,
        &QPushButton::clicked,
        this,
        &MainWindow::startSlowWorker
        );
    //再把信号连接到主窗口
    connect(
        collector_,
        &MockCollector::readingReady,
        this,
        &MainWindow::onReadingReady
        );

    // samplingTimer_ = new QTimer(this);

    // connect(
    //     samplingTimer_,
    //     &QTimer::timeout,
    //     this,
    //     &MainWindow::onSamplingTimeout
    //     );

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
    //让最后一条直接使用采集器的当前数据
    const std::array<DeviceReading, 4> readings{{
        {"T001", 26.5, true,  0.60},
        {"T002", 31.2, true,  0.72},
        {"T003", 28.0, false, 0.00},
        collector_->currentReading()
    }};
    // 用结构体保存练习数据，再逐行填入 Designer 创建的表格。

    // const std::array<DeviceReading, 4> readings{{
    //     {"T001", 26.5, true,  0.60},
    //     {"T002", 31.2, true,  0.72},
    //     {"T003", 28.0, false, 0.00},
    //     {"T004", 35.8, true,  0.85}
    // }};


    auto *table = ui->tableWidget;
    table->setRowCount(static_cast<int>(readings.size()));
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"设备编号", "温度", "压力","状态"});
    for (int row = 0; row < table->rowCount(); ++row) {
        const auto &reading = readings[row];
        deviceStore_.update(reading);
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(reading.deviceId)));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(reading.temperature, 'f', 1)));
        table->setItem(row, 2, new QTableWidgetItem(reading.online? QString::number(reading.pressureMPa, 'f', 2): "—"));
        table->setItem(row, 3, new QTableWidgetItem(reading.online ? "在线" : "离线"));
    }
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // updateTemperatureById("T004", 45.8);
    // updateTemperatureById("T004", t004Temperature_);

    statusBar()->showMessage("系统就绪");
    setWindowTitle("工业设备监控 - Day 09");
    appendLog("系统启动");


    tcpSocket_ = new QTcpSocket(this);

    // TCP 连接成功
    connect(tcpSocket_, &QTcpSocket::connected,
            this, [this]() {
                appendLog("TCP 连接成功");
            });

    // TCP 收到数据
    connect(tcpSocket_, &QTcpSocket::readyRead,
            this, &MainWindow::onTcpReadyRead);

    // TCP 连接断开
    connect(tcpSocket_, &QTcpSocket::disconnected,
            this, [this]() {
                appendLog("TCP 连接已断开");
                tcpBuffer_.clear();
            });

    // TCP 发生错误
    connect(
        tcpSocket_,
        &QAbstractSocket::errorOccurred,
        this,
        [this](QAbstractSocket::SocketError error) {

            if (error == QAbstractSocket::RemoteHostClosedError) {
                return; // 由 disconnected 统一记录断开
            }

            appendLog("TCP 错误：" + tcpSocket_->errorString());
        }
        );

    // 连接本地模拟服务器
    //tcpSocket_->connectToHost("127.0.0.1", 9000);
    // QString host =
    //     settings.value("tcp/host", "127.0.0.1").toString();

    // int port =
    //     settings.value("tcp/port", 9000).toInt();

    // if (host.isEmpty() || port < 1 || port > 65535)
    // {
    //     qWarning() << "TCP 配置非法，使用默认地址";
    //     host = "127.0.0.1";
    //     port = 9000;
    // }

    // tcpSocket_->connectToHost(
    //     host,
    //     static_cast<quint16>(port)
    //     );
    tcpSocket_->connectToHost(
        config.tcpHost,
        config.tcpPort
        );
    // AlarmManager testAlarm;

    // const std::array<double, 6> testValues{
    //     45.8, 46.1, 46.4, 45.5, 45.0, 46.2
    // };

    // for (double temperature : testValues)
    // {
    //     auto event =
    //         testAlarm.evaluate("T004", temperature);

    //     if (event == AlarmManager::Event::Triggered)
    //     {
    //         qDebug() << temperature << "高温报警触发";
    //     }
    //     else if (event == AlarmManager::Event::Recovered)
    //     {
    //         qDebug() << temperature << "报警恢复";
    //     }
    // }
    // DeviceStore store;

    // store.update({"T004", 46.6, true, 1.60});
    // store.update({"T004", 47.2, true, 1.75});

    // DeviceReading result{"", 0.0, false, 0.0};

    // if (store.get("T004", result))
    // {
    //     qDebug() << "设备：" << result.deviceId.c_str();
    //     qDebug() << "温度：" << result.temperature;
    //     qDebug() << "压力：" << result.pressureMPa;
    // }

    // qDebug() << "设备数量：" << store.count();


}



void MainWindow::applyReading(
    const DeviceReading& reading)
{
    // 1. 保存设备最新数据
    deviceStore_.update(reading);

    QString deviceId =
        QString::fromStdString(reading.deviceId);

    // 2. 从 DeviceStore 获取最新状态
    DeviceReading latest{"", 0.0, false, 0.0};

    if (!deviceStore_.get(deviceId, latest))
    {
        appendLog("设备数据读取失败：" + deviceId);
        return;
    }

    // 3. 更新设备表格
    updateDeviceReading(latest);

    // 4. 更新普通状态信息
    statusBar()->showMessage(
        QString("%1 当前温度：%2 ℃")
            .arg(deviceId)
            .arg(latest.temperature, 0, 'f', 1)
        );

    // 5. 交给独立报警模块判断
    handleAlarm(latest);

    // 临时调试：验证模型中的数据
    qDebug()
        << "DeviceStore 更新："
        << deviceId
        << latest.temperature
        << latest.pressureMPa
        << latest.online;
}


void MainWindow::handleAlarm(const DeviceReading& reading)
{
    // 离线设备暂不进行温度报警判断
    if (!reading.online)
        return;

    QString deviceId =
        QString::fromStdString(reading.deviceId);

    auto event = alarmManager_.evaluate(
        deviceId,
        reading.temperature
        );

    if (event == AlarmManager::Event::Triggered)
    {
        QString message =
            QString("高温报警：%1 温度 %2 ℃")
                .arg(deviceId)
                .arg(reading.temperature, 0, 'f', 1);

        appendLog(message);
        statusBar()->showMessage(message);
    }
    else if (event == AlarmManager::Event::Recovered)
    {
        QString message =
            QString("报警恢复：%1 温度 %2 ℃")
                .arg(deviceId)
                .arg(reading.temperature, 0, 'f', 1);

        appendLog(message);
        statusBar()->showMessage(message);
    }
}



void MainWindow::startSlowWorker()
{
    // 防止重复启动
    if (slowThread_ != nullptr)
        return;

    // 创建线程管理对象
    auto* thread = new QThread(this);

    // 创建工作对象，注意：不要指定父对象
    auto* worker = new SlowWorker();

    slowThread_ = thread;

    // 将 Worker 移动到工作线程
    worker->moveToThread(thread);

    ui->threadTestButton->setEnabled(false);
    appendLog("工作线程准备启动");

    // 线程启动后执行耗时任务
    connect(
        thread,
        &QThread::started,
        worker,
        &SlowWorker::doWork
        );

    // Worker 产生数据，主线程接收
    connect(
        worker,
        &SlowWorker::readingReady,
        this,
        [this](double temperature) {
            appendLog(
                QString("线程采样：%1 ℃")
                    .arg(temperature, 0, 'f', 1)
                );
        },
        Qt::QueuedConnection
        );

    // Worker 完成后，要求线程退出
    connect(
        worker,
        &SlowWorker::finished,
        thread,
        &QThread::quit
        );

    // 线程退出后清理 Worker
    connect(
        thread,
        &QThread::finished,
        worker,
        &QObject::deleteLater
        );

    // 线程结束后恢复测试按钮
    connect(
        thread,
        &QThread::finished,
        this,
        [this, thread]() {
            appendLog("工作线程已结束");

            ui->threadTestButton->setEnabled(true);
            slowThread_ = nullptr;

            thread->deleteLater();
        }
        );

    // 真正启动新线程
    thread->start();
}

void MainWindow::onTcpReadyRead()
{
    tcpBuffer_.append(tcpSocket_->readAll());

    int pos;

    while ((pos = tcpBuffer_.indexOf('\n')) != -1)
    {
        QByteArray line = tcpBuffer_.left(pos);

        tcpBuffer_.remove(0, pos + 1);

        if (line.endsWith('\r')) {
            line.chop(1);
        }

        if (line.isEmpty()) {
            continue;
        }

        appendLog(
            "TCP 收到：" + QString::fromUtf8(line)
            );


        QJsonParseError parseError;

        QJsonDocument doc =
            QJsonDocument::fromJson(line, &parseError);

        // 检查 JSON 语法是否正确
        if (parseError.error != QJsonParseError::NoError ||
            !doc.isObject())
        {
            appendLog("TCP JSON 解析失败：" +
                      parseError.errorString());
            continue;
        }

        QJsonObject obj = doc.object();

        // 检查字段是否存在、类型是否正确
        if (!obj.value("deviceId").isString() ||
            !obj.value("temperature").isDouble() ||
            !obj.value("pressureMPa").isDouble() ||
            !obj.value("online").isBool())
        {
            appendLog("TCP 数据字段错误");
            continue;
        }

        // 先提取设备 ID 和温度
        // QString deviceId = obj.value("deviceId").toString();
        // double temperature = obj.value("temperature").toDouble();
        // 使用 Day 06 已经实现的函数更新温度
        // updateTemperatureById(deviceId, temperature);

        DeviceReading reading{
            obj.value("deviceId").toString().toStdString(),
            obj.value("temperature").toDouble(),
            obj.value("online").toBool(),
            obj.value("pressureMPa").toDouble()
        };
        // updateDeviceReading(reading);
        // handleAlarm(reading);
        applyReading(reading);

    }

    if (tcpBuffer_.size() > 64 * 1024)
    {
        appendLog("TCP 消息过长，断开连接");
        tcpBuffer_.clear();
        tcpSocket_->disconnectFromHost();
    }
}


void MainWindow::updateDeviceReading(
    const DeviceReading& reading)
{
    auto* table = ui->tableWidget;

    QString deviceId =
        QString::fromStdString(reading.deviceId);

    for (int row = 0; row < table->rowCount(); ++row)
    {
        auto* idItem = table->item(row, 0);

        if (!idItem || idItem->text() != deviceId)
            continue;

        // 更新温度
        if (auto* item = table->item(row, 1))
            item->setText(
                QString::number(reading.temperature, 'f', 1)
                );

        // 更新压力
        if (auto* item = table->item(row, 2))
            item->setText(
                reading.online
                    ? QString::number(reading.pressureMPa, 'f', 2)
                    : "—"
                );

        // 更新在线状态
        if (auto* item = table->item(row, 3))
            item->setText(
                reading.online ? "在线" : "离线"
                );

        return;
    }

    appendLog("收到未知设备：" + deviceId);
}




void MainWindow::onReadingReady(
    const DeviceReading& reading)
{
    // QString deviceId =
    //     QString::fromStdString(reading.deviceId);

    // 更新对应设备的温度
    // updateTemperatureById(
    //     deviceId,
    //     reading.temperature
    //     );

    // 状态栏显示最新温度
    // statusBar()->showMessage(
    //     QString("%1 当前温度：%2 ℃")
    //         .arg(deviceId)
    //         .arg(reading.temperature, 0, 'f', 1)
    //     );


    // const double alarmThreshold = 46.0;

    // if (reading.temperature > alarmThreshold)
    // {
    //    handleAlarm(reading);
    // }

     applyReading(reading);
}


void MainWindow::appendLog(const QString& message)
{
    QString time = QDateTime::currentDateTime()
    .toString("HH:mm:ss");

    QString log = QString("[%1] %2")
                      .arg(time, message);  //把 %1 替换为时间，把 %2 替换为日志内容。

    ui->logEdit->appendPlainText(log);
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
   // obj["temperature"] = t004Temperature_;
    obj["temperature"] =
        collector_->currentReading().temperature;
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
    appendLog("开始采集");
    setCollecting(true);
    // 每 1000 毫秒触发一次
   // samplingTimer_->start(1000);
    collector_->start();
}

void MainWindow::stopCollection()
{
    appendLog("停止采集");
    // 停止定时更新
    //samplingTimer_->stop();
    collector_->stop();
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
    // updateTemperatureById(
    //     "T004",
    //     t004Temperature_
    //     );

    // 在状态栏显示最新温度
    // statusBar()->showMessage(
    //     QString("T004 当前温度：%1 ℃")
    //         .arg(t004Temperature_, 0, 'f', 1)
    //     );
}



MainWindow::~MainWindow()
{
    if (slowThread_ != nullptr)
    {
        slowThread_->requestInterruption();
        slowThread_->quit();
        slowThread_->wait();
    }

    delete ui;
    delete ui;
}
