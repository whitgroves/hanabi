#include "player.h"

Player::Player(QString username)
{
    this->username = username;
    this->tiles = QList<QString>();
}
