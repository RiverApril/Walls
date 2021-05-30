//
//  Actor.cpp
//  Walls
//
//  Created by Emily Atlee on 8/17/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "Actor.hpp"
#include "ActorPlayer.hpp"

string Actor::save() {
    return format("%f %f %f  %f %f %f  %f %f %f", 
        box.center.x, box.center.y, box.center.z, 
        box.radii.x, box.radii.y, box.radii.z, 
        lookRotation.x, lookRotation.y, lookRotation.z);
}

Actor* loadActor(stringstream& stream, Scene* scene) {
    string type;
    stream >> type;
    
    Actor* actor;

    if (type == "player") {
        actor = loadPlayer(stream, scene);
    } else {
        printf("loadActor failed, unknown actor type: %s\n", type.c_str());
        return nullptr;
    }

    AABB box = loadAABB(stream);
    vec3 lookRotation;
    stream >> lookRotation.x >> lookRotation.y >> lookRotation.z;
    actor->box = box;
    actor->lookRotation = lookRotation;
    return actor;
}
