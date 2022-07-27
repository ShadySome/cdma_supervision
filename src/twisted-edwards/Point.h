#ifndef POINT_H
#define POINT_H

#include "params.h"


namespace NTL {
    class Point {
    public:
        ZZ X, Y, Z;

        Point();

        Point(ZZ &X, ZZ &Y, ZZ &Z);
        
        friend bool operator==(Point& a, Point& b)
        {
            return a.X == b.X && a.Y == b.Y;
        }
    };
}

#endif // POINT_H