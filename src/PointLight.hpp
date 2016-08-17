//
//  PointLight.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/17/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include "Headers.hpp"

struct PointLight{
    vec3 position = vec3(0, 0, 0);
    vec3 color = vec3(1, 1, 1);
    float intensity = 10;
};

#endif /* PointLight_hpp */
