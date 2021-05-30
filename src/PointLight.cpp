//
//  PointLight.cpp
//  Walls
//
//  Created by Emily Atlee on 8/17/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "PointLight.hpp"

string PointLight::save() {
    return format("%f %f %f  %f %f %f  %f %f %f  %f %f %f ", 
        position.x, position.y, position.z,
        specular.r, specular.g, specular.b,
        diffuse.r, diffuse.g, diffuse.b,
        attenuation.x, attenuation.y, attenuation.z);
}

PointLight* loadPointLight(stringstream& stream) {
    PointLight* pointLight = new PointLight();

    stream >> pointLight->position.x >> pointLight->position.y >> pointLight->position.z;
    stream >> pointLight->specular.r >> pointLight->specular.g >> pointLight->specular.b;
    stream >> pointLight->diffuse.r >> pointLight->diffuse.g >> pointLight->diffuse.b;
    stream >> pointLight->attenuation.x >> pointLight->attenuation.y >> pointLight->attenuation.z;

    return pointLight;
}