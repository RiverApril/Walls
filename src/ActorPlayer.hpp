//
//  ActorPlayer.hpp
//  Walls
//
//  Created by Emily Atlee on 8/17/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef ActorPlayer_hpp
#define ActorPlayer_hpp

#include "Actor.hpp"
#include "PointLight.hpp"

class Scene;

class ActorPlayer : public Actor{
    
    
public:
    
    vec3 velocity = vec3(0, 0, 0);
    bool onGround = false;
    
    Scene* activeScene;
    
    ActorPlayer(Scene* scene);
    ~ActorPlayer();
    
    virtual void render(GraphicsWindow* gw);
    bool tryToMove(vec3 diff);

    virtual string save();
    
};

ActorPlayer* loadPlayer(stringstream& stream, Scene* scene);

#endif /* ActorPlayer_hpp */
