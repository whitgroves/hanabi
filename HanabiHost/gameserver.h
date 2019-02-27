#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>

class GameServer : public QObject
{
    Q_OBJECT

public:
    explicit GameServer(QObject *parent = nullptr);

signals:
    void dataReceived(QByteArray);

private slots:
    void sessionOpened();
    void newConnection();
    void disconnected();
    void readyRead();

private:
    QTcpServer *server = nullptr;
    QHash<QTcpSocket*, QByteArray*> buffers; //store data until the whole block is received
    QHash<QTcpSocket*, qint32*> sizes; //store size of each block to verify it's been received completely
    QNetworkSession *networkSession = nullptr;
};

#endif // GAMESERVER_H
