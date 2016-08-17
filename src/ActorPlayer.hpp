//
//  ActorPlayer.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/17/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef ActorPlayer_hpp
#define ActorPlayer_hpp

#include "Actor.hpp"
#include "PointLight.hpp"

class Scene;

class ActorPlayer : public Actor{
    
    
public:
    PointLight light;
    Scene* activeScene;
    
    ActorPlayer(Scene* scene);
    ~ActorPlayer();
    
    virtual void render(GraphicsWindow* gw);
    bool tryToMove(vec3 diff);
    
};

#endif /* ActorPlayer_hpp */
