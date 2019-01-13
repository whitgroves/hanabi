#ifndef TILE_H
#define TILE_H

enum Colors {ALL, RED, BLUE, YELLOW, GREEN, WHITE};

class Tile
{
private:
    Colors color;
    int number;
    bool colorKnown, numberKnown;

public:
    Tile(Colors, int);
};

#endif // TILE_H
