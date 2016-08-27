//
//  Hologram.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/26/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Hologram.hpp"
#include "GraphicsWindow.hpp"


void Hologram::render(GraphicsWindow *gw){
    
    matrix = mat4();
    matrix = translate(matrix, pos);
    if(lockOnPlayer){
        matrix = rotate(matrix, -gw->player->lookRotation.y, vec3(0, 1.0f, 0));
    }else{
        matrix = rotate(matrix, rot.z, vec3(0, 0, 1.0f));
        matrix = rotate(matrix, rot.x, vec3(1.0f, 0, 0));
        matrix = rotate(matrix, rot.y, vec3(0, 1.0f, 0));
    }
    matrix = scale(matrix, vec3(0.01f, -0.01f, 1));
    matrix = gw->matrixProjection * gw->matrixCamera * matrix;
    glUniformMatrix4fv(gw->flatShader.getUniformLocation("viewMatrix"), 1, false, &matrix[0][0]);
    flat->render();
    
}