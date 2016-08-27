//
//  Prop.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/17/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Prop_hpp
#define Prop_hpp

#include "Headers.hpp"
#include "Mesh.hpp"
#include "Geometry.hpp"
#include "Model.hpp"

class GraphicsWindow;

class Prop{
public:
    
    AABB box;
    
    mat4 matrix;
    Mesh* mesh;
    bool mustScale;
    
    Prop(Mesh* mesh, vec3 pos) : mesh(mesh){
        box.center = pos;
        mustScale = false;
    }
    
    Prop(vec3 scale, vec3 pos){
        mesh = Models::cube;
        box.radii = scale;
        box.center = pos;
        mustScale = true;
    }
    
    virtual ~Prop(){}
    
    virtual void render(GraphicsWindow* gw);
    
    
};

#endif /* Prop_hpp */
