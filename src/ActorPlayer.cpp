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
    velocity = vec3(0.0, 0.0, 0.0);
    box.radii = vec3(0.125, 0.5, 0.125);
    box.center = vec3(0.0, 0.0, 0.0);
}

ActorPlayer::~ActorPlayer(){
    
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
    //printf("A Pos: %f, %f, %f\n", box.center.x, box.center.y, box.center.z);
    
    lookRotation.y += gw->mousePosDelta.x * gw->settings->lookSpeed;
    lookRotation.x += gw->mousePosDelta.y * gw->settings->lookSpeed;
    if(lookRotation.x > radians(89.9f)){
        lookRotation.x = radians(89.9f);
    }
    if(lookRotation.x < -radians(89.9f)){
        lookRotation.x = -radians(89.9f);
    }
    if(lookRotation.y > two_pi<float>()){
        lookRotation.y -= two_pi<float>();
    }
    if(lookRotation.y < 0){
        lookRotation.y += two_pi<float>();
    }
    
    vec3 diff = vec3(0.0, 0.0, 0.0);
    
    const float jumpSpeed = 0.1;
    const float moveSpeed = onGround ? 0.02 : 0.001;
    const float gravity = 0.005;
    const float groundInvFriction = 0.5;
    const float airInvFriction = 0.99;
    
    //printf("B Pos: %f, %f, %f\n", box.center.x, box.center.y, box.center.z);
    
    if(!gw->consoleActive){
        if(onGround){
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
            diff.z -= cos(lookRotation.y-half_pi<float>()) * moveSpeed;
            diff.x += sin(lookRotation.y-half_pi<float>()) * moveSpeed;
        }
        if(glfwGetKey(gw->window, GLFW_KEY_D) == GLFW_PRESS){
            diff.z -= cos(lookRotation.y+half_pi<float>()) * moveSpeed;
            diff.x += sin(lookRotation.y+half_pi<float>()) * moveSpeed;
        }
    }
    
    
    //printf("C Pos: %f, %f, %f\n", box.center.x, box.center.y, box.center.z);
    
    velocity += diff;
    
    //printf("D Pos: %f, %f, %f\n", box.center.x, box.center.y, box.center.z);
    
    if(!tryToMove(velocity)){
        float maxDiff = 0.125;
        vec3 xdiff = vec3(clamp(velocity.x, -maxDiff, maxDiff), 0, 0);
        vec3 ydiff = vec3(0, clamp(velocity.y, -maxDiff, maxDiff), 0);
        vec3 zdiff = vec3(0, 0, clamp(velocity.z, -maxDiff, maxDiff));
        for(float i = 0; i < abs(velocity.x); i+=maxDiff){
            if(!tryToMove(xdiff)){
                velocity.x = 0;
                break;
            }
        }
        for(float i = 0; i < abs(velocity.y); i+=maxDiff){
            if(!tryToMove(ydiff)){
                velocity.y = 0;
                break;
            }
        }
        for(float i = 0; i < abs(velocity.z); i+=maxDiff){
            if(!tryToMove(zdiff)){
                velocity.z = 0;
                break;
            }
        }
    }
    
    //printf("E Pos: %f, %f, %f\n", box.center.x, box.center.y, box.center.z);
    
    onGround = false;
    
    AABB yBox = box;
    if(velocity.y == 0){
        yBox.center.y -= 0.1;
    }else{
        yBox.center.y += clamp(velocity.y, -.1, .1);
    }
    
    for(Prop* p : activeScene->props){
        if(p->box.intersects(yBox) && !p->box.intersects(box)){
            onGround = velocity.y <= 0;
            velocity.y = 0;
            break;
        }
    }
    
    if(onGround){
        velocity.x *= groundInvFriction;
        velocity.z *= groundInvFriction;
    }else{
        velocity.x *= airInvFriction;
        velocity.z *= airInvFriction;
        velocity.y -= gravity;
    }
    
    gw->cameraPosition = box.center+vec3(0, box.radii.y, 0);
    gw->cameraRotation = lookRotation;
    
    //printf("F Pos: %f, %f, %f\n", box.center.x, box.center.y, box.center.z);
}
