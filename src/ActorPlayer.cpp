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
            break;
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
    
    float jumpSpeed = 0.2;
    float moveSpeed = 0.001;
    
    if(onGround){
        moveSpeed = 0.05;
        if(glfwGetKey(gw->window, GLFW_KEY_SPACE) == GLFW_PRESS){
            diff.y += jumpSpeed;
        }
    }
    
    if(glfwGetKey(gw->window, GLFW_KEY_W) == GLFW_PRESS){
        diff.z -= cos(lookRotation.y) * moveSpeed;
        diff.x += sin(lookRotation.y) * moveSpeed;
    }
    if(glfwGetKey(gw->window, GLFW_KEY_S) == GLFW_PRESS){
        diff.z -= cos(lookRotation.y) * -moveSpeed;
        diff.x += sin(lookRotation.y) * -moveSpeed;
    }
    if(glfwGetKey(gw->window, GLFW_KEY_A) == GLFW_PRESS){
        diff.z -= cos(lookRotation.y-radians(90.0f)) * moveSpeed;
        diff.x += sin(lookRotation.y-radians(90.0f)) * moveSpeed;
    }
    if(glfwGetKey(gw->window, GLFW_KEY_D) == GLFW_PRESS){
        diff.z -= cos(lookRotation.y-radians(90.0f)) * -moveSpeed;
        diff.x += sin(lookRotation.y-radians(90.0f)) * -moveSpeed;
    }
    //if(glfwGetKey(gw->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    //    diff.y -= 0.1;
    //}
    
    velocity += diff;
    
    
    if(!tryToMove(velocity)){
        float maxDiff = 0.1;
        vec3 xdiff = vec3(clamp(velocity.x, -maxDiff, maxDiff), 0, 0);
        vec3 ydiff = vec3(0, clamp(velocity.y, -maxDiff, maxDiff), 0);
        vec3 zdiff = vec3(0, 0, clamp(velocity.z, -maxDiff, maxDiff));
        for(float i = 0; i < abs(velocity.x); i+=maxDiff){
            tryToMove(xdiff);
        }
        for(float i = 0; i < abs(velocity.y); i+=maxDiff){
            tryToMove(ydiff);
        }
        for(float i = 0; i < abs(velocity.z); i+=maxDiff){
            tryToMove(zdiff);
        }
    }
    
    onGround = false;
    
    AABB yBox = box;
    yBox.center.y += clamp(velocity.y, -.1, .1);
    
    for(Prop* p : activeScene->props){
        if(p->box.intersects(yBox) && !p->box.intersects(box)){
            onGround = velocity.y < 0;
            velocity.y = 0;
            break;
        }
    }
    
    if(onGround){
        velocity.x *= 0.5;
        velocity.z *= 0.5;
    }else{
        velocity.x *= 0.99;
        velocity.z *= 0.99;
        velocity.y -= 0.01;
    }
    
    gw->cameraPosition = box.center;
    gw->cameraRotation = lookRotation;
    light.position = box.center;
}