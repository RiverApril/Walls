//
//  Ray.hpp
//  Walls
//
//  Created by Emily Atlee on 8/18/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include "Headers.hpp"

class Prop;

enum Side{
    xm, xp, ym, yp, zm, zp
};

vec3 sideNormal(Side side);
vec3 sideMask(Side side);
vec3 sideInvMask(Side side);

struct RayData{
    
    RayData(){}
    
    vec3 position;
    bool hit;
    Prop* prop;
    Side side;
    
};

#endif /* Ray_hpp */
