#ifndef _COORD_H
#define _COORD_H

class Coord
{
    float _x;
    float _y;
    float _z;

public:
    Coord(float x = 0, float y = 0, float z = 0)
    {
        _x = x;
        _y = y;
        _z = z;
    };
    float calculateAverage()
    {
        return (_x + _y + _z) / 3;
    }
};

#endif