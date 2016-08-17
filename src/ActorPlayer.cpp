//
//  ActorPlayer.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/17/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "ActorPlayer.hpp"
#include "Scene.hpp"
#include "GraphicsWindow.hpp"
#include "Geometry.hpp"



ActorPlayer::ActorPlayer(Scene* scene){
    activeScene = scene;
    activeScene->pointLights.push_back(&light);
    
    box.radii = vec3(0.25, 0.5, 0.25);
    
}

ActorPlayer::~ActorPlayer(){
    removeFromVector(activeScene->pointLights, &light);
}

bool ActorPlayer::tryToMove(vec3 diff){
    bool hit = false;
    AABB diffBox = box;
    diffBox.center += diff;
    
    for(Prop* p : activeScene->props){
        if(p->box.intersects(diffBox) && !p->box.intersects(box)){
            hit = true;
        }
    }
    
    if(!hit){
        box.center += diff;
    }
    return !hit;
}

void ActorPlayer::render(GraphicsWindow* gw){
    
    lookRotation.y += gw->mousePosDelta.x * gw->settings->lookSpeed;
    lookRotation.x += gw->mousePosDelta.y * gw->settings->lookSpeed;
    if(lookRotation.x > radians(89.0f)){
        lookRotation.x = radians(89.0f);
    }
    if(lookRotation.x < -radians(89.0f)){
        lookRotation.x = -radians(89.0f);
    }
    
    vec3 diff;
    
    if(glfwGetKey(gw->window, GLFW_KEY_W) == GLFW_PRESS){
        diff.z -= cos(lookRotation.y) * 0.1;
        diff.x += sin(lookRotation.y) * 0.1;
    }
    if(glfwGetKey(gw->window, GLFW_KEY_S) == GLFW_PRESS){
        diff.z -= cos(lookRotation.y) * -0.1;
        diff.x += sin(lookRotation.y) * -0.1;
    }
    if(glfwGetKey(gw->window, GLFW_KEY_A) == GLFW_PRESS){
        diff.z -= cos(lookRotation.y-radians(90.0f)) * 0.1;
        diff.x += sin(lookRotation.y-radians(90.0f)) * 0.1;
    }
    if(glfwGetKey(gw->window, GLFW_KEY_D) == GLFW_PRESS){
        diff.z -= cos(lookRotation.y-radians(90.0f)) * -0.1;
        diff.x += sin(lookRotation.y-radians(90.0f)) * -0.1;
    }
    if(glfwGetKey(gw->window, GLFW_KEY_SPACE) == GLFW_PRESS){
        diff.y += 0.1;
    }
    if(glfwGetKey(gw->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        diff.y -= 0.1;
    }
    
    
    if(!tryToMove(diff)){
        vec3 xdiff = vec3(diff.x, 0, 0);
        vec3 ydiff = vec3(0, diff.y, 0);
        vec3 zdiff = vec3(0, 0, diff.z);
        tryToMove(xdiff);
        tryToMove(ydiff);
        tryToMove(zdiff);
    }
    
    gw->cameraPosition = box.center;
    gw->cameraRotation = lookRotation;
    light.position = box.center;
}