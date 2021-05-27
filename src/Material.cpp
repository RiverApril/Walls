//
//  Material.cpp
//  Walls
//
//  Created by Emily Atlee on 5/20/17.
//  Copyright © 2017 Emily Atlee. All rights reserved.
//

#include "Material.hpp"


Material::Material(string filename, vec3 ambient, vec3 diffuse, vec3 specular, float shininess) : name(filename), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess){
    
    
}
namespace Materials{
    
    Material* defaultMaterial;
    
    void initMaterials(){
        defaultMaterial = new Material("default", vec3(.1, .1, .1), vec3(1, 1, 1), vec3(1, 1, 1), 1);
    }
}
