//
//  Vertex.hpp
//  Walls
//
//  Created by Emily Atlee on 8/15/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef Vertex_hpp
#define Vertex_hpp

#include "Headers.hpp"
#include "Geometry.hpp"

struct Vertex{
    
    Vertex(){}
    
    Vertex(vec3 Position, vec4 Color, vec3 Normal, vec2 UV) : Position(Position), Color(Color), Normal(Normal), UV(UV){}
    
    vec3 Position = vec3(0, 0, 0);
    vec4 Color = vec4(0, 0, 0, 0);
    vec3 Normal = vec3(0, 0, 0);
    vec2 UV = vec2(0, 0);
    
    const static int strideToPosition = 0;
    const static int strideToColor    = sizeof(float) * (3);
    const static int strideToNormal   = sizeof(float) * (3+4);
    const static int strideToUV       = sizeof(float) * (3+4+3);
    const static int strideToEnd      = sizeof(float) * (3+4+3+2);
    
};

bool operator==(const Vertex& a, const Vertex& b);
bool operator!=(const Vertex& a, const Vertex& b);

struct FlatVertex{
    
    FlatVertex(){}
    
    FlatVertex(vec3 Position, vec4 Color, vec2 UV) : Position(Position), Color(Color), UV(UV){}
    
    vec3 Position = vec3(0, 0, 0);
    vec4 Color = vec4(0, 0, 0, 0);
    vec2 UV = vec2(0, 0);
    
    const static int strideToPosition = 0;
    const static int strideToColor    = sizeof(float) * (3);
    const static int strideToUV       = sizeof(float) * (3+4);
    const static int strideToEnd      = sizeof(float) * (3+4+2);
};

bool operator==(const FlatVertex& a, const FlatVertex& b);
bool operator!=(const FlatVertex& a, const FlatVertex& b);

#endif /* Vertex_hpp */
