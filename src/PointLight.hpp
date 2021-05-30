//
//  PointLight.hpp
//  Walls
//
//  Created by Emily Atlee on 8/17/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include "Headers.hpp"

struct PointLight{
    vec3 position = vec3(0, 0, 0);
    vec3 specular = vec3(1, 1, 1);
    vec3 diffuse = vec3(1, 1, 1);
    vec3 attenuation = vec3(1, 0.25, 0.1);

    string save();
};

PointLight* loadPointLight(stringstream& stream);

#endif /* PointLight_hpp */
