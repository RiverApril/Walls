//
//  Prop.cpp
//  Walls
//
//  Created by Emily Atlee on 8/17/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "Prop.hpp"
#include "GraphicsWindow.hpp"


void Prop::render(GraphicsWindow* gw){
    
    mat4 matrix = identity<mat4>();
    matrix = translate(matrix, box.center);
    if(mustScale){
        matrix = scale(matrix, box.radii);
    }else{
        box.radii = mesh->radii;
    }
    
    glUniformMatrix4fv(gw->worldShader.getUniformLocation("modelMatrix"), 1, false, &matrix[0][0]);
    
    glUniform3f(gw->worldShader.getUniformLocation("material.diffuse"), material->diffuse.r, material->diffuse.g, material->diffuse.b);
    glUniform3f(gw->worldShader.getUniformLocation("material.ambient"), material->ambient.r, material->ambient.g, material->ambient.b);
    glUniform3f(gw->worldShader.getUniformLocation("material.specular"), material->specular.r, material->specular.g, material->specular.b);
    glUniform1f(gw->worldShader.getUniformLocation("material.shininess"), material->shininess);
    
    mesh->draw(false);
}

void Prop::renderShadows(GraphicsWindow* gw){
    
    mat4 matrix = identity<mat4>();
    matrix = translate(matrix, box.center);
    if(mustScale){
        matrix = scale(matrix, box.radii);
    }
    
    glUniformMatrix4fv(gw->shadowShader.getUniformLocation("modelMatrix"), 1, false, &matrix[0][0]);
    
    mesh->draw(true);
}

string Prop::save() {
    Model* model = dynamic_cast<Model*>(mesh);
    string modelName = "unknownModel";
    if (model) {
        modelName = model->name;
    } else {
        printf("unable to save prop mesh as it is not a model.\n");
    }
    return format("%s %s %s %d", modelName.c_str(), material->name.c_str(), box.save().c_str(), mustScale?1:0);
}

Prop* loadProp(stringstream& stream) {
    string modelName;
    string materialName;
    AABB box;
    int mustScale;

    stream >> modelName;
    stream >> materialName;
    box = loadAABB(stream);
    stream >> mustScale;

    if(Models::models.find(modelName) == Models::models.end()){
        printf("loadProp failed, unknown model: %s\n", modelName.c_str());
        return nullptr;
    }

    if(Materials::materials.find(materialName) == Materials::materials.end()){
        printf("loadProp failed, unknown material: %s\n", materialName.c_str());
        return nullptr;
    }

    Model* model = Models::models.at(modelName);
    Material* material = Materials::materials.at(materialName);

    return new Prop(model, material, box, mustScale!=0);
}
