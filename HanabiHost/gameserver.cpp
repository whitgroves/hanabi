#include "gameserver.h"

GameServer::GameServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any, 8081))
    {
        qDebug() << "server failed to start.";
    }
    else
    {
        qDebug() << "server started successfully.";
    }
}

void GameServer::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();

    socket->write("Good evening. Thank you for joining us on Skylark Tonight!\r\n");
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
}
