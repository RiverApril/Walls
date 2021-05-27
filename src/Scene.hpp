//
//  Scene.hpp
//  Walls
//
//  Created by Emily Atlee on 8/17/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include "Headers.hpp"
#include "Actor.hpp"
#include "Prop.hpp"
#include "PointLight.hpp"
#include "Ray.hpp"

class GraphicsWindow;

class Scene {
    
public:
    vector<Actor*> actors;
    
    vector<Prop*> props;
    
    vector<PointLight*> pointLights;
    
    void render(GraphicsWindow* gw);
    
    RayData rayProps(vec3 origin, vec3 look, float far = INFINITY);
    
    
};

#endif /* Scene_hpp */
