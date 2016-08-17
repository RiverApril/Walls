//
//  GraphicsWindow.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef GraphicsWindow_hpp
#define GraphicsWindow_hpp

#include "Headers.hpp"
#include "Settings.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "Scene.hpp"
#include "ActorPlayer.hpp"

class GraphicsWindow{
public:
    
    Settings* settings;
    string title;
    
    bool running;
    
    
    GLFWwindow* window;
    ShaderProgram worldShader;
    
    
    mat4 matrixProjection;
    mat4 matrixCamera;
    
    mat4 matrixView;
    mat3 normalMatrix;
    
    vec3 cameraPosition;
    vec3 cameraRotation;
    
    dvec2 mousePos;
    dvec2 mousePosPrev;
    dvec2 mousePosDelta;
    
    ActorPlayer* player;
    
    Scene* activeScene;
    
    long tick = 0;
    
    
    GraphicsWindow(Settings* settings, string title) : settings(settings), title(title){
        if(initWindow()){
            running = true;
        }
    }
    
    bool initWindow();
    void startLoop();
    
};

extern GraphicsWindow* graphicsWindowInstance;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif /* GraphicsWindow_hpp */
