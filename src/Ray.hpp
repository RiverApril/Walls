//
//  Ray.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/18/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include "Prop.hpp"

struct RayData{
    
    RayData(){}
    
    vec3 position;
    bool hit;
    Prop* prop;
    
};

#endif /* Ray_hpp */
