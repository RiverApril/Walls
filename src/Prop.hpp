//
//  Prop.hpp
//  Walls
//
//  Created by Emily Atlee on 8/17/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef Prop_hpp
#define Prop_hpp

#include "Headers.hpp"
#include "Mesh.hpp"
#include "Geometry.hpp"
#include "Model.hpp"
#include "Material.hpp"

class GraphicsWindow;

class Prop{
public:
    
    Mesh* mesh;
    Material* material;
    AABB box;
    bool mustScale;
    
    Prop(Mesh* mesh, Material* material, vec3 pos) : mesh(mesh), material(material){
        box.center = pos;
        mustScale = false;
    }
    
    Prop(vec3 scale, vec3 pos){
        mesh = Models::cube;
        material = Materials::defaultMaterial;
        box.radii = scale;
        box.center = pos;
        mustScale = true;
    }

    Prop(Mesh* mesh, Material* material, AABB box, bool mustScale): mesh(mesh), material(material), box(box), mustScale(mustScale){}
    
    virtual ~Prop(){}
    
    virtual void renderShadows(GraphicsWindow* gw);
    virtual void render(GraphicsWindow* gw);
    
    virtual string save();
    
};

Prop* loadProp(stringstream& stream);

#endif /* Prop_hpp */
