#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

class GameServer : public QObject
{
    Q_OBJECT
public:
    explicit GameServer(QObject *parent = nullptr);

signals:

public slots:
    void newConnection();

private:
    QTcpServer *server;
};

#endif // GAMESERVER_H
