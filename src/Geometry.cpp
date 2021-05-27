//
//  Geometry.cpp
//  Walls
//
//  Created by Emily Atlee on 8/15/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "Geometry.hpp"
#include "Ray.hpp"


Point2 operator+(const Point2& a, const Point2& b){return Point2(a.x+b.x, a.y+b.y);}
Point2 operator-(const Point2& a, const Point2& b){return Point2(a.x-b.x, a.y-b.y);}
Point2 operator*(const Point2& a, const Point2& b){return Point2(a.x*b.x, a.y*b.y);}
Point2 operator/(const Point2& a, const Point2& b){return Point2(a.x/b.x, a.y/b.y);}

Point2 operator+(const Point2& a, const int b){return Point2(a.x+b, a.y+b);}
Point2 operator-(const Point2& a, const int b){return Point2(a.x-b, a.y-b);}
Point2 operator*(const Point2& a, const int b){return Point2(a.x*b, a.y*b);}
Point2 operator/(const Point2& a, const int b){return Point2(a.x/b, a.y/b);}



bool AABB::intersects(AABB& other){
    return abs(center.x - other.center.x) < (radii.x + other.radii.x) &&
           abs(center.y - other.center.y) < (radii.y + other.radii.y) &&
           abs(center.z - other.center.z) < (radii.z + other.radii.z);
}

bool AABB::intersects(vec3 origin, vec3 look, vec3& hit, float& dist, Side& side) {
    
    vec3 invLook = 1.0f / look;
    
    float tx1 = (center.x - radii.x - origin.x)*invLook.x;
    float tx2 = (center.x + radii.x - origin.x)*invLook.x;
    
    float txmin = std::min(tx1, tx2);
    float txmax = std::max(tx1, tx2);
    
    float ty1 = (center.y - radii.y - origin.y)*invLook.y;
    float ty2 = (center.y + radii.y - origin.y)*invLook.y;
    
    float tymin = std::min(ty1, ty2);
    float tymax = std::max(ty1, ty2);
    
    float tz1 = (center.z - radii.z - origin.z)*invLook.z;
    float tz2 = (center.z + radii.z - origin.z)*invLook.z;
    
    float tzmin = std::min(tz1, tz2);
    float tzmax = std::max(tz1, tz2);
    
    float tmin = std::max(txmin, std::max(tymin, tzmin));
    float tmax = std::min(txmax, std::min(tymax, tzmax));
    
    if(tmax >= tmin && tmin >= 0){
        float tm = tmin;
        hit = origin+(tm*look);
        
        vec3 vDistMin = abs(hit - (center - radii));
        vec3 vDistMax = abs(hit - (center + radii));
        
        side = xm;
        float fMinDist = vDistMin.x;
        
        if(vDistMax.x < fMinDist){
            side = xp;
            fMinDist = vDistMax.x;
        }
        
        if(vDistMin.y < fMinDist){
            side = ym;
            fMinDist = vDistMin.y;
        }
        if(vDistMax.y < fMinDist){
            side = yp;
            fMinDist = vDistMax.y;
        }
        
        if(vDistMin.z < fMinDist){
            side = zm;
            fMinDist = vDistMin.z;
        }
        if(vDistMax.z < fMinDist){
            side = zp;
            fMinDist = vDistMax.z;
        }
        
        dist = abs(tm);
        return true;
    }else{
        return false;
    }
}

float roundTo(float v, float m){
    return round(v * m) / m;
}

vec3 roundTo(vec3 v, float m){
    return vec3(roundTo(v.x, m), roundTo(v.y, m), roundTo(v.z, m));
}

