//
//  Material.hpp
//  Walls
//
//  Created by Braeden Atlee on 5/20/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include "Headers.hpp"

struct Material {
    
    string name;
    
    vec3 ambient = vec3(0, 0, 0);
    vec3 diffuse = vec3(1, 1, 1);
    vec3 specular = vec3(1, 1, 1);
    float shininess = 0;
    
    Material(string filename, vec3 ambient, vec3 diffuse, vec3 specular, float shininess);
    ~Material(){}
    
};

namespace Materials{
    extern Material* defaultMaterial;
    
    void initMaterials();
}

#endif /* Material_hpp */
