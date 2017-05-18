//
//  Prop.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/17/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Prop.hpp"
#include "GraphicsWindow.hpp"


void Prop::render(GraphicsWindow* gw){
    
    matrix = mat4();
    matrix = translate(matrix, box.center);
    if(mustScale){
        matrix = scale(matrix, box.radii);
    }else{
        box.radii = mesh->radii;
    }
    
    gw->matrixView = gw->matrixProjection * gw->matrixCamera * matrix;
    gw->normalMatrix = mat3(transpose(inverse(matrix)));
    glUniformMatrix4fv(gw->worldShader.getUniformLocation("viewMatrix"), 1, false, &gw->matrixView[0][0]);
    glUniformMatrix4fv(gw->worldShader.getUniformLocation("modelMatrix"), 1, false, &matrix[0][0]);
    glUniformMatrix3fv(gw->worldShader.getUniformLocation("normalMatrix"), 1, false, &gw->normalMatrix[0][0]);
    mesh->draw();
}
