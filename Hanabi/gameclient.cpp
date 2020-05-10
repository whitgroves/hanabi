#include <QtNetwork>

#include "gameclient.h"

GameClient::GameClient(QObject *parent) : QObject(parent), socket(new QTcpSocket(this))
{
    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_5_10);

    connect(socket, &QIODevice::readyRead, this, &GameClient::readMessage);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &GameClient::logError);

    connect(socket, &QAbstractSocket::connected, this, &GameClient::logSocketConnected);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        //get saved netconfig, or default if there isn't one
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &GameClient::sessionOpened);

        networkSession->open();
    }

    qDebug() << "Client started. Requesting first message...";

    requestMessage();
}

void GameClient::requestMessage() {
    socket->abort();
    socket->connectToHost(QHostAddress::LocalHost, 8081);
}

void GameClient::logSocketConnected() {
    qDebug() << "Successfully connected to host.";
}

void GameClient::readMessage() {
    in.startTransaction();

    QString nextMessage;
    in >> nextMessage;

    if (!in.commitTransaction()) {
        qDebug() << "There was an error while reading the socket. Waiting for more data...";

        return;
    }

    QByteArray responseBlock;
    QDataStream out(&responseBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    QString responseString = "This is a debug message.";
    out << responseString;

    socket->write(responseBlock);

    qDebug() << "Message sent: " + responseString;

    if (nextMessage == currentMessage) {
        QTimer::singleShot(0, this, &GameClient::requestMessage);

        qDebug() << "No new data. Requesting new message...";

        return;
    }

    currentMessage = nextMessage;

    qDebug() << "Message received: " + currentMessage;
}

void GameClient::logError(QAbstractSocket::SocketError socketError) {
    switch(socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "The host was not found.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "Connection refused. Please check that HanabiHost is running.";
        break;
    default:
        qDebug() << "Unexpected error: " + socket->errorString();
    }
}

void GameClient::sessionOpened() {
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

bool GameClient::writeData(QByteArray data) {
    if(socket->state() == QAbstractSocket::ConnectedState) {
        // determine size of package, and write that first as qint32 (4 bytes)
        QByteArray sizeData;
        QDataStream temp(&sizeData, QIODevice::ReadWrite);
        temp << data.size();

        // write and send
        socket->write(sizeData);
        socket->write(data);

        return socket->waitForBytesWritten();
    }
    else {
        return false;
    }
}
