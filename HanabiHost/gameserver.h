#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QDebug>

class QTcpServer;
class QNetworkSession;

class GameServer : public QObject
{
    Q_OBJECT

public:
    explicit GameServer(QObject *parent = nullptr);

private slots:
    void sessionOpened();
    void sendMessage();

private:
    QTcpServer *server = nullptr;
    QNetworkSession *networkSession = nullptr;
};

#endif // GAMESERVER_H
