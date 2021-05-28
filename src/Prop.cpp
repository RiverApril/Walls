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
    
    matrix = identity<mat4>();
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
    
    matrix = identity<mat4>();
    matrix = translate(matrix, box.center);
    if(mustScale){
        matrix = scale(matrix, box.radii);
    }
    
    glUniformMatrix4fv(gw->shadowShader.getUniformLocation("modelMatrix"), 1, false, &matrix[0][0]);
    
    mesh->draw(true);
}
