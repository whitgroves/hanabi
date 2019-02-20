#include "tile.h"

Tile::Tile(Colors color, int number)
{
    this->color = color;
    this->number = number;
    this->colorKnown = false;
    this->numberKnown = false;
}
