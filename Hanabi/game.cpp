#include "game.h"

Game::Game(QList<Player> players)
{
    this->players = players;
    this->startGame();
}

void Game::startGame()
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

void Game::runGame()
{

}

void Game::setupTiles()
{

}

void Game::dealTiles()
{

}

void Game::giveColorHint()
{

}

void Game::giveNumberHint()
{

}

void Game::playTile()
{

}

void Game::discardTile()
{

}

void Game::endGame()
{

}
