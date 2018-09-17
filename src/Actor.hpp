//
//  Actor.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/17/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Actor_hpp
#define Actor_hpp

#include "Headers.hpp"
#include "Geometry.hpp"

class GraphicsWindow;

class Actor{
public:
    AABB box;
    
    vec3 lookRotation = vec3(0, 0, 0);
    
    Actor(){};
    virtual ~Actor(){}
    
    virtual void render(GraphicsWindow* gw){}
    
};

#endif /* Actor_hpp */
