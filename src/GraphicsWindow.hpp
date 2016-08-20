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
    vec3 cameraLook;
    
    bool okayToPlace = false;
    Prop* selectedProp = nullptr;
    
    dvec2 mousePos;
    dvec2 mousePosPrev;
    dvec2 mousePosDelta;
    
    ActorPlayer* player;
    
    Scene* activeScene;
    
    long tick = 0;
    
    Prop* propToPlace = nullptr;
    
    int frames;
    long startTime;
    long passedTime;
    
    
    GraphicsWindow(Settings* settings, string title) : settings(settings), title(title){
        if(initWindow()){
            running = true;
        }
    }
    
    bool initWindow();
    void startLoop();
    void makeProjectionMatrix();
    
    void keyEvent(int key, int scancode, int action, int mods);
    
};

extern GraphicsWindow* graphicsWindowInstance;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void resizeCallback(GLFWwindow* window, int width, int height);

#endif /* GraphicsWindow_hpp */
