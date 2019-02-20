#include "gameinstance.h"

GameInstance::GameInstance(QList<Player> players)
{
    this->players = players;
    this->startGame();
}

void GameInstance::startGame()
{
    this->tiles = QList<Tile>();
    this->stacks = QHash<Colors, int>();
    this->livesLeft = 3;
    this->hintsLeft = 8;
    this->discardedTiles = QList<Tile>();
    this->lastRound = false;

    //TODO: refactor to make this more sexy
    this->stacks[RED] = 0;
    this->stacks[BLUE] = 0;
    this->stacks[YELLOW] = 0;
    this->stacks[GREEN] = 0;
    this->stacks[WHITE] = 0;

    this->setupTiles();
    this->dealTiles();

    while(this->lastRound == false)
    {
        this->runGame();
    }
    // needs to run one more time. TODO: clean up this filthy hack
    this->runGame();
    this->endGame();
}

void GameInstance::runGame()
{
    foreach(Player player, this->players)
    {
        foreach(Tile tile, player.tiles)
        {
            // TODO: send signal to UI to populate the player's tiles
        }

        foreach(Player otherPlayer, this->players)
        {
            if(&otherPlayer != &player)
            {
                foreach(Tile tile, otherPlayer.tiles)
                {
                    //TODO: send signal to UI to populate the other players' team view
                }
            }
        }
    }
}

void GameInstance::setupTiles()
{

}

void GameInstance::dealTiles()
{

}

void GameInstance::giveColorHint()
{

}

void GameInstance::giveNumberHint()
{

}

void GameInstance::playTile()
{

}

void GameInstance::discardTile()
{

}

void GameInstance::endGame()
{

}
