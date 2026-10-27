
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 获取主窗口中央区域
    QWidget *center = centralWidget();

    // 为中央区域创建垂直布局
    auto *layout = new QVBoxLayout(center);

    // 创建状态标签
    auto *label = new QLabel("设备状态：待机", center);

    // 设置文本居中
    label->setAlignment(Qt::AlignCenter);

    // 将标签添加到布局中
    layout->addWidget(label);

    // 设置底部状态栏
    statusBar()->showMessage(
        "系统就绪"
        );
}

MainWindow::~MainWindow()
{
    delete ui;
}
