#include <QtNetwork>
#include <QtCore>

#include "gameserver.h"

GameServer::GameServer(QObject *parent) : QObject(parent)
{
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // pull saved netconfig, or default if no config
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &GameServer::sessionOpened);

        qDebug() << "Opening network session";

        networkSession->open();
    } else {
        sessionOpened();
    }

    connect(server, &QTcpServer::newConnection, this, &GameServer::sendMessage);
}

void GameServer::sessionOpened()
{
    // Save the netconfig
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;

        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::LocalHost, 8081)) {
        qDebug() << "Unable to start the server";

        return;
    }
    QString ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    QString serverPort = QString::number(server->serverPort());

    qDebug() << "Server is running on port " + serverPort;
}

void GameServer::sendMessage()
{
    qDebug() << "New connection received. Sending message...";

    QByteArray messageBlock;
    QDataStream out(&messageBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    QString messageString = "Welcome to Skylark Tonight! I'm your host, Dave Skylark, and today we have a connected server!";

    out << messageString;

    QTcpSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);

    clientConnection->write(messageBlock);
    clientConnection->disconnectFromHost();

    qDebug() << "Message sent: " + messageString;
}
