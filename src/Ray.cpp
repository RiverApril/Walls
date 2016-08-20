//
//  Ray.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/18/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Ray.hpp"
#include "Prop.hpp"

vec3 sideNormal(Side side){
    switch(side){
        case xm: return vec3(-1,  0,  0);
        case xp: return vec3( 1,  0,  0);
        case ym: return vec3( 0, -1,  0);
        case yp: return vec3( 0,  1,  0);
        case zm: return vec3( 0,  0, -1);
        case zp: return vec3( 0,  0,  1);
    }
}

vec3 sideMask(Side side){
    switch(side){
        case xm: return vec3( 1,  0,  0);
        case xp: return vec3( 1,  0,  0);
        case ym: return vec3( 0,  1,  0);
        case yp: return vec3( 0,  1,  0);
        case zm: return vec3( 0,  0,  1);
        case zp: return vec3( 0,  0,  1);
    }
}

vec3 sideInvMask(Side side){
    switch(side){
        case xm: return vec3( 0,  1,  1);
        case xp: return vec3( 0,  1,  1);
        case ym: return vec3( 1,  0,  1);
        case yp: return vec3( 1,  0,  1);
        case zm: return vec3( 1,  1,  0);
        case zp: return vec3( 1,  1,  0);
    }
}