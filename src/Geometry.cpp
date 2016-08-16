//
//  Geometry.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Geometry.hpp"


Point2 operator+(const Point2& a, const Point2& b){return Point2(a.x+b.x, a.y+b.y);}
Point2 operator-(const Point2& a, const Point2& b){return Point2(a.x-b.x, a.y-b.y);}
Point2 operator*(const Point2& a, const Point2& b){return Point2(a.x*b.x, a.y*b.y);}
Point2 operator/(const Point2& a, const Point2& b){return Point2(a.x/b.x, a.y/b.y);}

Point2 operator+(const Point2& a, const int b){return Point2(a.x+b, a.y+b);}
Point2 operator-(const Point2& a, const int b){return Point2(a.x-b, a.y-b);}
Point2 operator*(const Point2& a, const int b){return Point2(a.x*b, a.y*b);}
Point2 operator/(const Point2& a, const int b){return Point2(a.x/b, a.y/b);}




