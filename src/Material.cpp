//
//  Material.cpp
//  Walls
//
//  Created by Emily Atlee on 5/20/17.
//  Copyright © 2017 Emily Atlee. All rights reserved.
//

#include "Material.hpp"


Material::Material(string name, vec3 ambient, vec3 diffuse, vec3 specular, float shininess) : name(name), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess){
    Materials::materials.insert(pair<string, Material*>(name, this));
}
namespace Materials{
    
    map<string, Material*> materials;
    Material* defaultMaterial;
    
    void initMaterials(){
        defaultMaterial = new Material("default", vec3(.1, .1, .1), vec3(1, 1, 1), vec3(1, 1, 1), 1);
    }
}
