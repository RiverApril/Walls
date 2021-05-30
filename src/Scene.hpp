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
class ActorPlayer;

class Scene {

private:
    vector<Actor*> actors;
    
    vector<Prop*> props;
    
    vector<PointLight*> pointLights;

    bool shadowsDirty;
    
public:

    Scene(): shadowsDirty(true){}
    
    void renderShadows(GraphicsWindow* gw);
    void render(GraphicsWindow* gw);

    void addProp(Prop* prop);
    void addActor(Actor* actor);
    void addPointLight(PointLight* pointLight);

    void removeProp(Prop* prop);
    void removeActor(Actor* actor);
    void removePointLight(PointLight* pointLight);

    string save();

    vector<Prop*> getProps();
    ActorPlayer* getPlayer();
    
    RayData rayProps(vec3 origin, vec3 look, float far = INFINITY);
    
};

Scene* loadScene(stringstream& stream);

#endif /* Scene_hpp */
