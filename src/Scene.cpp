//
//  Scene.cpp
//  Walls
//
//  Created by Emily Atlee on 8/17/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "Scene.hpp"
#include "GraphicsWindow.hpp"
#include "Ray.hpp"
#include "Texture.hpp"

const float shadowAspect = 1.0f; // shadow resolution is square
const float shadowNear = 0.01f;
const float shadowFar = 25.0f;
const mat4 shadowProj = perspective(radians(90.0f), shadowAspect, shadowNear, shadowFar);

void Scene::renderShadows(GraphicsWindow *gw){

    if(shadowsDirty){
        shadowsDirty = false;

        // start using shadow shader
        gw->shadowShader.use();

        glViewport(0, 0, gw->settings->shadowResolution, gw->settings->shadowResolution);

        glBindFramebuffer(GL_FRAMEBUFFER, gw->depthMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, gw->depthCubemap, 0);
        glClear(GL_DEPTH_BUFFER_BIT);

        int count = std::min((int)pointLights.size(), 16);
        for (int i = 0; i < count; i++) {

            vec3 lightPos = pointLights[i]->position;

            vector<mat4> shadowTransforms;
            shadowTransforms.push_back(shadowProj * lookAt(lightPos, lightPos + vec3( 1.0, 0.0, 0.0), vec3(0.0,-1.0, 0.0)));
            shadowTransforms.push_back(shadowProj * lookAt(lightPos, lightPos + vec3(-1.0, 0.0, 0.0), vec3(0.0,-1.0, 0.0)));
            shadowTransforms.push_back(shadowProj * lookAt(lightPos, lightPos + vec3( 0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)));
            shadowTransforms.push_back(shadowProj * lookAt(lightPos, lightPos + vec3( 0.0,-1.0, 0.0), vec3(0.0, 0.0,-1.0)));
            shadowTransforms.push_back(shadowProj * lookAt(lightPos, lightPos + vec3( 0.0, 0.0, 1.0), vec3(0.0,-1.0, 0.0)));
            shadowTransforms.push_back(shadowProj * lookAt(lightPos, lightPos + vec3( 0.0, 0.0,-1.0), vec3(0.0,-1.0, 0.0)));
            
            glUniform1f(gw->shadowShader.getUniformLocation("farPlane"), shadowFar);
            glUniform3f(gw->shadowShader.getUniformLocation("lightPos"), lightPos.x, lightPos.y, lightPos.z);
            glUniformMatrix4fv(gw->shadowShader.getUniformLocation("shadowMatrices[0]"), 6, false, &shadowTransforms[0][0][0]);
            glUniform1i(gw->shadowShader.getUniformLocation("lightIndex"), i);

            //draw props
            for(Prop* p : props){
                p->renderShadows(gw);
            }
            //
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void Scene::render(GraphicsWindow *gw){
    
    glUniform1i(gw->worldShader.getUniformLocation("activeTexture"), 0);
    
    //set lights
    
    int count = std::min((int)pointLights.size(), 16);
    glUniform1i(gw->worldShader.getUniformLocation("pointLightCount"), count);
    glUniform1i(gw->worldShader.getUniformLocation("pointLightFarPlane"), shadowFar);
    for(int i=0;i<count;i++){
        string l = "pointLights["+to_string(i)+"]";
        glUniform3f(gw->worldShader.getUniformLocation(l+".position"), pointLights[i]->position.x, pointLights[i]->position.y, pointLights[i]->position.z);
        glUniform3f(gw->worldShader.getUniformLocation(l+".specular"), pointLights[i]->specular.r, pointLights[i]->specular.g, pointLights[i]->specular.b);
        glUniform3f(gw->worldShader.getUniformLocation(l+".diffuse"), pointLights[i]->diffuse.r, pointLights[i]->diffuse.g, pointLights[i]->diffuse.b);
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

void Scene::addProp(Prop* prop){
    props.push_back(prop);
    shadowsDirty = true;
}

void Scene::addActor(Actor* actor){
    actors.push_back(actor);
    shadowsDirty = true;
}

void Scene::addPointLight(PointLight* pointLight){
    pointLights.push_back(pointLight);
    shadowsDirty = true;
}

void Scene::removeProp(Prop* prop){
    removeFromVector(props, prop);
    shadowsDirty = true;
}

void Scene::removeActor(Actor* actor){
    removeFromVector(actors, actor);
    shadowsDirty = true;
}

void Scene::removePointLight(PointLight* pointLight){
    removeFromVector(pointLights, pointLight);
    shadowsDirty = true;
}

vector<Prop*> Scene::getProps() {
    return props;
}

ActorPlayer* Scene::getPlayer() {
    for(Actor* actor : actors){
        ActorPlayer* player = dynamic_cast<ActorPlayer*>(actor);
        if(player){
            return player;
        }
    }
    return nullptr;
}


string Scene::save() {
    string data = "";

    data += "scene v0\n";
    for(Actor* actor : actors){
        data += "actor ";
        data += actor->save();
        data += "\n";
    }
    for(Prop* prop : props){
        data += "prop ";
        data += prop->save();
        data += "\n";
    }
    for(PointLight* pointLight : pointLights){
        data += "pointLight ";
        data += pointLight->save();
        data += "\n";
    }

    return data;
}

Scene* loadScene(stringstream& stream) {
    Scene* scene = new Scene();

    string magicWord;
    stream >> magicWord;
    if(magicWord != "scene"){
        printf("loadScene failed, not a scene file\n");
        delete scene;
        return nullptr;
    }
    string version;
    stream >> version;
    if (version == "v0") {
        while (stream) {
            string header;
            stream >> header;
            if (header.size() == 0) {
                continue;
            } else if (header == "actor") {
                Actor* actor = loadActor(stream, scene);
                if (actor) {
                    scene->addActor(actor);
                } else {
                    printf("loadScene failed, failed to load actor\n");
                    delete scene;
                    return nullptr;
                }
            } else if (header == "prop") {
                Prop* prop = loadProp(stream);
                if (prop) {
                    scene->addProp(prop);
                } else {
                    printf("loadScene failed, failed to load prop\n");
                    delete scene;
                    return nullptr;
                }
            } else if (header == "pointLight") {
                PointLight* pointLight = loadPointLight(stream);
                if (pointLight) {
                    scene->addPointLight(pointLight);
                } else {
                    printf("loadScene failed, failed to load pointLight\n");
                    delete scene;
                    return nullptr;
                }
            } else {
                printf("loadScene failed, unknown header: %s\n", header.c_str());
                delete scene;
                return nullptr;
            }
        }
        return scene;
    } else {
        printf("loadScene failed, unknown version: %s\n", version.c_str());
        delete scene;
        return nullptr;
    }
}