#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "common.h"
#include "gameclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug() << "window up, starting background client";
    GameClient client;
    return a.exec();
}
