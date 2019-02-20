#include <QCoreApplication>

#include "common.h"
#include "gameserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GameServer mServer;
    return a.exec();
}
