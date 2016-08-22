//
//  Gui.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/21/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Gui.hpp"


void Gui::compile(){
    
    vertices.clear();
    indices.clear();
    
    /*
     
     0 -- 1
     |  ,'|
     |,'  |
     2 -- 3
     
     */
    
    
    for(Sprite* sprite : sprites){
        GuiVertex v0 = GuiVertex(vec3(sprite->center.x-sprite->radii.x, sprite->center.y-sprite->radii.y, sprite->z), sprite->color, vec2(sprite->UV.x, sprite->UV.y));
        GuiVertex v1 = GuiVertex(vec3(sprite->center.x+sprite->radii.x, sprite->center.y-sprite->radii.y, sprite->z), sprite->color, vec2(sprite->UV.z, sprite->UV.y));
        GuiVertex v2 = GuiVertex(vec3(sprite->center.x-sprite->radii.x, sprite->center.y+sprite->radii.y, sprite->z), sprite->color, vec2(sprite->UV.x, sprite->UV.w));
        GuiVertex v3 = GuiVertex(vec3(sprite->center.x+sprite->radii.x, sprite->center.y+sprite->radii.y, sprite->z), sprite->color, vec2(sprite->UV.z, sprite->UV.w));
        unsigned int ind = (unsigned int)vertices.size();
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        indices.push_back(ind+0);
        indices.push_back(ind+1);
        indices.push_back(ind+3);
        indices.push_back(ind+0);
        indices.push_back(ind+3);
        indices.push_back(ind+2);
    }
    
    for(TextSprite* text : textSprites){
        vec2 pos = text->pos;
        for(int i=0;i<text->text.size();i++){
            
            if(text->text[i] == '\n'){
                pos.y += 8;
                pos.x = text->pos.x;
                continue;
            }
            vec2 size = vec2(8, 8);
            vec4 UV;
            int xx = (text->text[i]-32)%16;
            int yy = (text->text[i]-32)/16;
            UV.x = xx * (1.0f/32);
            UV.y = yy * (1.0f/32);
            UV.z = (xx+1) * (1.0f/32);
            UV.w = (yy+1) * (1.0f/32);
            
            GuiVertex v0 = GuiVertex(vec3(pos.x       , pos.y       , text->z), text->color, vec2(UV.x, UV.y));
            GuiVertex v1 = GuiVertex(vec3(pos.x+size.x, pos.y       , text->z), text->color, vec2(UV.z, UV.y));
            GuiVertex v2 = GuiVertex(vec3(pos.x       , pos.y+size.y, text->z), text->color, vec2(UV.x, UV.w));
            GuiVertex v3 = GuiVertex(vec3(pos.x+size.x, pos.y+size.y, text->z), text->color, vec2(UV.z, UV.w));
            unsigned int ind = (unsigned int)vertices.size();
            vertices.push_back(v0);
            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);
            indices.push_back(ind+0);
            indices.push_back(ind+1);
            indices.push_back(ind+2);
            indices.push_back(ind+1);
            indices.push_back(ind+3);
            indices.push_back(ind+2);
            
            pos.x += 8;
        }
    }
    
    if(compiled){
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }
    
    elementCount = (GLsizei)indices.size();
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, GuiVertex::strideToEnd * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elementCount, &indices[0], GL_STATIC_DRAW);
    
    vertices.clear();
    indices.clear();
    
    //To Fix glEnableVertexAttribArray() returning error:
    GLuint vaoId = 0;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);
    //
    
    compiled = true;
}

void Gui::render(){
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, GuiVertex::strideToEnd, (void *)GuiVertex::strideToPosition);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, GuiVertex::strideToEnd, (void *)GuiVertex::strideToColor);
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, GuiVertex::strideToEnd, (void *)GuiVertex::strideToUV);
    
    glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
}

