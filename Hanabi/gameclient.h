#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QDataStream>
#include <QObject>
#include <QDebug>
#include <QTcpSocket>

class QTcpSocket;
class QNetworkSession;

class GameClient : public QObject
{
    Q_OBJECT

public:
    explicit GameClient(QObject *parent = nullptr);

public slots:
    void requestMessage();
    void readMessage();
    void logError(QAbstractSocket::SocketError socketError);
    void sessionOpened();
    void logSocketConnected();

private:
    QTcpSocket *socket = nullptr;
    QDataStream in;
    QString currentMessage;

    QNetworkSession *networkSession = nullptr;
};

#endif // GAMECLIENT_H
