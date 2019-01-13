#ifndef PLAYER_H
#define PLAYER_H

#include "tile.h"
#include <QString>
#include <QList>

class Player
{
public:
    Player(QString username);
    QString username;
    QList<QString> tiles;
};

#endif // PLAYER_H
