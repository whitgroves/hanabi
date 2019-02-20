#ifndef PLAYER_H
#define PLAYER_H

#include "tile.h"
#include <QString>
#include <QList>

class Player
{
public:
    Player(QString username);
    QList<Tile> tiles;

private:
    QString username;
};

#endif // PLAYER_H
