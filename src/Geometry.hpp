//
//  Geometry.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

struct Point2{
    int x, y;
    
    Point2(int x, int y) : x(x), y(y){}
    Point2() : x(0), y(0){}
};

Point2 operator+(const Point2& a, const Point2& b);
Point2 operator-(const Point2& a, const Point2& b);
Point2 operator*(const Point2& a, const Point2& b);
Point2 operator/(const Point2& a, const Point2& b);

Point2 operator+(const Point2& a, const int b);
Point2 operator-(const Point2& a, const int b);
Point2 operator*(const Point2& a, const int b);
Point2 operator/(const Point2& a, const int b);

#endif /* Geometry_hpp */