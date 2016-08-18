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



bool AABB::intersects(AABB& other){
    return abs(center.x - other.center.x) <= (radii.x + other.radii.x) &&
           abs(center.y - other.center.y) <= (radii.y + other.radii.y) &&
           abs(center.z - other.center.z) <= (radii.z + other.radii.z);
}

bool AABB::intersects(vec3 origin, vec3 look, vec3& hit, float& dist) {
    
    vec3 invLook = 1.0f / look;
    
    float tx1 = (center.x - radii.x - origin.x)*invLook.x;
    float tx2 = (center.x + radii.x - origin.x)*invLook.x;
    
    float tmin = std::min(tx1, tx2);
    float tmax = std::max(tx1, tx2);
    
    float ty1 = (center.y - radii.y - origin.y)*invLook.y;
    float ty2 = (center.y + radii.y - origin.y)*invLook.y;
    
    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));
    
    float tz1 = (center.z - radii.z - origin.z)*invLook.z;
    float tz2 = (center.z + radii.z - origin.z)*invLook.z;
    
    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));
    
    if(tmax >= tmin){
        float tm = tmin;
        if(tmin < 0){
            tm = -tmax;
        }
        hit = origin+(tm*look);
        dist = abs(tm);
        return true;
    }else{
        return false;
    }
}

