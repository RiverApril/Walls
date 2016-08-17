//
//  Vertex.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Vertex_hpp
#define Vertex_hpp

#include "Headers.hpp"
#include "Geometry.hpp"

struct Vertex{
    
    Vertex(){}
    
    Vertex(vec3 Position, vec4 Color, vec3 Normal) : Position(Position), Color(Color), Normal(Normal){}
    
    vec3 Position;
    vec4 Color;
    vec3 Normal;
    
    const static int strideToPosition = 0;
    const static int strideToColor    = sizeof(float) * (3);
    const static int strideToNormal   = sizeof(float) * (3+4);
    const static int strideToEnd      = sizeof(float) * (3+4+3);
    
};

bool operator==(const Vertex& a, const Vertex& b);
bool operator!=(const Vertex& a, const Vertex& b);

#endif /* Vertex_hpp */
