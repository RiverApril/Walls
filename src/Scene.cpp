//
//  Scene.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/17/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Scene.hpp"
#include "GraphicsWindow.hpp"
#include "Ray.hpp"
#include "Texture.hpp"


void Scene::render(GraphicsWindow *gw){
    
    glUniform1i(gw->worldShader.getUniformLocation("activeTexture"), 0);
    
    //set lights
    
    int count = std::min((int)pointLights.size(), 16);
    glUniform1i(gw->worldShader.getUniformLocation("pointLightCount"), count);
    for(int i=0;i<count;i++){
        string l = "pointLights["+to_string(i)+"]";
        glUniform3f(gw->worldShader.getUniformLocation(l+".position"), pointLights[i]->position.x, pointLights[i]->position.y, pointLights[i]->position.z);
        glUniform3f(gw->worldShader.getUniformLocation(l+".specular"), pointLights[i]->specular.r, pointLights[i]->specular.g, pointLights[i]->specular.b);
        glUniform3f(gw->worldShader.getUniformLocation(l+".attenuation"), pointLights[i]->attenuation.x, pointLights[i]->attenuation.y, pointLights[i]->attenuation.z);
    }
    //
    
    //draw props
    for(Prop* p : props){
        p->render(gw);
    }
    //
    
    //draw actors
    for(Actor* a : actors){
        a->render(gw);
    }
    //
    
}

RayData Scene::rayProps(vec3 origin, vec3 look, float far){
    RayData data;
    data.hit = false;
    float close = far;
    for(Prop* p : props){
        vec3 hit;
        float dist;
        Side side;
        if(p->box.intersects(origin, look, hit, dist, side)){
            if(dist < close){
                data.hit = true;
                data.position = hit;
                data.prop = p;
                data.side = side;
                close = dist;
            }
        }
    }
    return data;
}



