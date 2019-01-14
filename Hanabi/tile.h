#ifndef TILE_H
#define TILE_H

enum Colors {ALL, RED, BLUE, YELLOW, GREEN, WHITE};

class Tile
{
public:
    Tile(Colors, int);

private:
    Colors color;
    int number;
    bool colorKnown, numberKnown;
};

#endif // TILE_H
