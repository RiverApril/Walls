//
//  Scene.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/17/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include "Headers.hpp"
#include "Actor.hpp"
#include "Prop.hpp"
#include "PointLight.hpp"

class GraphicsWindow;

class Scene {
    
public:
    vector<Actor*> actors;
    
    vector<Prop*> props;
    
    vector<PointLight*> pointLights;
    
    void render(GraphicsWindow* gw);
    
    
};

#endif /* Scene_hpp */
