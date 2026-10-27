#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //1.创建 Qt GUI 应用对象
    QApplication a(argc, argv);
    //2.创建主窗口对象
    MainWindow w;

    w.setWindowTitle("工业设备监控 - Day 05");//修改窗口标题
    w.resize(520, 320); //设置窗口初始宽高。
    w.show();

    //3.显示主窗口
    w.show();
    //4.a.exec()启动事件循环；return	事件循环结束后返回退出状态
    return a.exec();
}
