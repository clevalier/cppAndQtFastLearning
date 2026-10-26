#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //1.创建 Qt GUI 应用对象
    QApplication a(argc, argv);
    //2.创建主窗口对象
    MainWindow w;
    //3.显示主窗口
    w.show();
    //4.a.exec()启动事件循环；return	事件循环结束后返回退出状态
    return QApplication::exec();
}
