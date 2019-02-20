#ifndef GAME_H
#define GAME_H

#include "player.h"
#include <QList>
#include <QHash>

class GameInstance
{
public:
    GameInstance(QList<Player> players);

private:
    void startGame();
    void runGame();
    void setupTiles();
    void dealTiles();
    void giveColorHint();
    void giveNumberHint();
    void playTile();
    void discardTile();
    void endGame();

    QList<Player> players;
    QList<Tile> tiles;
    QHash<Colors, int> stacks;
    int livesLeft;
    int hintsLeft;
    QList<Tile> discardedTiles;
    bool lastRound;
};

#endif // GAME_H
