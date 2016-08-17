//
//  GraphicsWindow.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "GraphicsWindow.hpp"

GraphicsWindow* graphicsWindowInstance;


bool GraphicsWindow::initWindow(){
    
    if(!glfwInit()){
        fprintf(stderr, "ERROR: Failed to start GLFW\n");
        return false;
    }
    
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(settings->windowSize.x, settings->windowSize.y, title.c_str(), NULL, NULL);
    
    if(!window){
        fprintf(stderr, "ERROR: Failed to create window with GLFW\n");
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported: %s\n", version);
    
    glfwSetKeyCallback(window, keyCallback);
    
    worldShader.addShader(Shader("shaders/world.vert.glsl", GL_VERTEX_SHADER));
    worldShader.addShader(Shader("shaders/world.frag.glsl", GL_FRAGMENT_SHADER));
    worldShader.link();
    
    matrixProjection = perspectiveFov<float>(radians(80.0f), settings->windowSize.x, settings->windowSize.y, 0.1f, 1000);
    pointLights.push_back(&playerLight);
    playerLight.color = vec3(1, 1, 1);
    playerLight.intensity = 10;
    
    testModel = new Model("models/cutcube.obj");
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0, 0.5, 1.0, 1.0);
    
    return true;
    
}

void GraphicsWindow::startLoop(){
    while(running && !glfwWindowShouldClose(window)){
        tick++;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //user input
        mousePosPrev = mousePos;
        glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
        dvec2 mousePosDelta = mousePos - mousePosPrev;
        cameraRotation.y += mousePosDelta.x * settings->lookSpeed;
        cameraRotation.x += mousePosDelta.y * settings->lookSpeed;
        if(cameraRotation.x > radians(89.0f)){
            cameraRotation.x = radians(89.0f);
        }
        if(cameraRotation.x < -radians(89.0f)){
            cameraRotation.x = -radians(89.0f);
        }
        
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            cameraPosition.z -= cos(cameraRotation.y) * 0.1;
            cameraPosition.x += sin(cameraRotation.y) * 0.1;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            cameraPosition.z -= cos(cameraRotation.y) * -0.1;
            cameraPosition.x += sin(cameraRotation.y) * -0.1;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            cameraPosition.z -= cos(cameraRotation.y-radians(90.0f)) * 0.1;
            cameraPosition.x += sin(cameraRotation.y-radians(90.0f)) * 0.1;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            cameraPosition.z -= cos(cameraRotation.y-radians(90.0f)) * -0.1;
            cameraPosition.x += sin(cameraRotation.y-radians(90.0f)) * -0.1;
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            cameraPosition.y += 0.1;
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            cameraPosition.y -= 0.1;
        }
        //
        
        
        //start using world shader
        worldShader.use();
        
        //set camera
        matrixCamera = mat4();
        matrixCamera = rotate<float>(matrixCamera, cameraRotation.x, vec3(1.0f, 0, 0));
        matrixCamera = rotate<float>(matrixCamera, cameraRotation.y, vec3(0, 1.0f, 0));
        matrixCamera = translate(matrixCamera, -cameraPosition);
        //
        
        //set lights
        
        playerLight.position = cameraPosition;
        
        int count = std::min((int)pointLights.size(), 16);
        glUniform1i(worldShader.getUniformLocation("pointLightCount"), count);
        for(int i=0;i<count;i++){
            string l = "pointLights["+to_string(i)+"]";
            glUniform3f(worldShader.getUniformLocation(l+".position"), pointLights[i]->position.x, pointLights[i]->position.y, pointLights[i]->position.z);
            glUniform3f(worldShader.getUniformLocation(l+".color"), pointLights[i]->color.r, pointLights[i]->color.g, pointLights[i]->color.b);
            glUniform1f(worldShader.getUniformLocation(l+".intensity"), pointLights[i]->intensity);
        }
        //
        
        //draw model
        mat4 modelMatrix = mat4(); // will be set by object using model
        matrixView = matrixProjection * matrixCamera * modelMatrix;
        normalMatrix = mat3(transpose(inverse(modelMatrix)));
        glUniformMatrix4fv(worldShader.getUniformLocation("viewMatrix"), 1, false, &matrixView[0][0]);
        glUniformMatrix4fv(worldShader.getUniformLocation("modelMatrix"), 1, false, &modelMatrix[0][0]);
        glUniformMatrix3fv(worldShader.getUniformLocation("normalMatrix"), 1, false, &normalMatrix[0][0]);
        testModel->draw();
        //
        
        
        
        glfwPollEvents();
        glfwSwapBuffers(window);
        
        GLenum error = glGetError();
        if(error != 0){
            printf("Uh oh, we've got a GL error somewhere: 0x%X\n", error);
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    printf("Key: %d\n", key);
}