#include <QtNetwork>
#include <QtCore>

#include "gameserver.h"

GameServer::GameServer(QObject *parent) : QObject(parent)
{
    // TODO: remove netconfig block if unneeded.
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
}

void GameServer::sessionOpened()
{
    // Save the netconfig TODO: remove if possible
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
    connect(server, &QTcpServer::newConnection, this, &GameServer::newConnection);

    if (!server->listen(QHostAddress::LocalHost, 8081)) {
        qDebug() << "Unable to start the server";

        return;
    }
    QString ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    QString serverPort = QString::number(server->serverPort());

    qDebug() << "Server is running on port " + serverPort;
}

void GameServer::newConnection()
{
    while (server->hasPendingConnections())
    {
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));

        QByteArray *buffer = new QByteArray();
        qint32 *size = new qint32(0);
        buffers.insert(socket, buffer);
        sizes.insert(socket, size);

        qDebug() << "New connection received. Sending message...";

        QByteArray messageBlock;
        QDataStream out(&messageBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_10);

        QString messageString = "Welcome to Skylark Tonight! I'm your host, Dave Skylark, and today we have a connected server!";
        out << messageString;

        socket->write(messageBlock);

        qDebug() << "Message sent: " + messageString;
    }
}

void GameServer::disconnected() {
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *size = sizes.value(socket);

    socket->deleteLater();
    delete buffer;
    delete size;
}

void GameServer::readyRead() {

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *size = sizes.value(socket);
    qint32 dataSize = *size;

    while (socket->bytesAvailable() > 0) {

        buffer->append(socket->readAll());

        while ((dataSize == 0 && buffer->size() >= 4) || (dataSize > 0 && buffer->size() >= dataSize)) { // all packages are prefixed with a 4-byte size value

            if (dataSize == 0 && buffer->size() >= 4) {
                QByteArray tempSource = buffer->mid(0, 4);
                QDataStream temp(&tempSource, QIODevice::ReadWrite);
                temp >> dataSize;
                *size = dataSize;
                buffer->remove(0, 4);
            }

            if (dataSize > 0 && buffer->size() >= dataSize) {
                QByteArray data = buffer->mid(0, dataSize);
                buffer->remove(0, dataSize);
                dataSize = 0;
                *size = dataSize;

                qDebug() << data; //debugging

                emit dataReceived(data);

                socket->disconnectFromHost();
            }
        }
    }
}
