//
//  Hologram.hpp
//  Walls
//
//  Created by Emily Atlee on 8/26/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef Hologram_hpp
#define Hologram_hpp

#include "Headers.hpp"
#include "Flat.hpp"

class GraphicsWindow;

class Hologram{
    
public:
    bool lockOnPlayer = true;
    vec3 rot = vec3(0, 0, 0);
    vec3 pos = vec3(0, 0, 0);
    mat4 matrix;
    
    Flat* flat;
    
    void render(GraphicsWindow* gw);
    
};

#endif /* Hologram_hpp */
