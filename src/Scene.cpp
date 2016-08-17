//
//  Scene.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/17/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Scene.hpp"
#include "GraphicsWindow.hpp"


void Scene::render(GraphicsWindow *gw){
    
    
    //set lights
    int count = std::min((int)pointLights.size(), 16);
    glUniform1i(gw->worldShader.getUniformLocation("pointLightCount"), count);
    for(int i=0;i<count;i++){
        string l = "pointLights["+to_string(i)+"]";
        glUniform3f(gw->worldShader.getUniformLocation(l+".position"), pointLights[i]->position.x, pointLights[i]->position.y, pointLights[i]->position.z);
        glUniform3f(gw->worldShader.getUniformLocation(l+".color"), pointLights[i]->color.r, pointLights[i]->color.g, pointLights[i]->color.b);
        glUniform1f(gw->worldShader.getUniformLocation(l+".intensity"), pointLights[i]->intensity);
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