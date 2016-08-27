//
//  Hologram.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/26/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Hologram_hpp
#define Hologram_hpp

#include "Headers.hpp"
#include "Flat.hpp"

class GraphicsWindow;

class Hologram{
    
public:
    bool lockOnPlayer = true;
    vec3 rot;
    vec3 pos;
    mat4 matrix;
    
    Flat* flat;
    
    void render(GraphicsWindow* gw);
    
};

#endif /* Hologram_hpp */
