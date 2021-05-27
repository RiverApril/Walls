//
//  GraphicsWindow.hpp
//  Walls
//
//  Created by Emily Atlee on 8/15/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef GraphicsWindow_hpp
#define GraphicsWindow_hpp

#include "Headers.hpp"
#include "Settings.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "Scene.hpp"
#include "ActorPlayer.hpp"
#include "Flat.hpp"
#include "Hologram.hpp"

class GraphicsWindow{
public:
    
    Settings* settings;
    string title;
    
    bool running;
    
    
    GLFWwindow* window;
    ShaderProgram worldShader;
    ShaderProgram flatShader;
    
    
    mat4 matrixProjection = identity<mat4>();
    mat4 matrixCamera = identity<mat4>();
    
    mat4 matrixView = identity<mat4>();
    mat3 normalMatrix = identity<mat3>();
    
    mat4 matrixHud = identity<mat4>();
    
    vec3 cameraPosition = vec3(0, 0, 0);
    vec3 cameraRotation = vec3(0, 0, 0);
    vec3 cameraLook = vec3(0, 0, 0);
    
    bool okayToPlace = false;
    Prop* selectedProp = nullptr;
    RayData lookRay;
    
    dvec2 mousePos;
    dvec2 mousePosPrev;
    dvec2 mousePosDelta;
    
    ActorPlayer* player;
    
    Scene* activeScene;
    
    long tick = 0;
    
    Prop* propToPlace = nullptr;
    
    Flat* hud;
    TextSprite* consoleSprite;
    
    Hologram* testHologram;
    
    int frames;
    long startTime;
    long passedTime;
    int fps = 0;
    
    vector<string> consoleOutput;
    string consoleInput = "";
    bool consoleActive = false;
    int consoleLines = 0;
    
    bool debugActive = false;
    
    
    GraphicsWindow(Settings* settings, string title) : settings(settings), title(title){
        if(initWindow()){
            running = true;
        }
    }
    
    bool initWindow();
    void startLoop();
    void makeProjectionMatrix();
    void proccessCommand(string command);
    void consoleAdd(string add);
    
    void keyEvent(int key, int scancode, int action, int mods);
    void mouseButtonEvent(int button, int action, int mods);
    void charEvent(int codepoint);
    
};

extern GraphicsWindow* graphicsWindowInstance;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void charCallback(GLFWwindow* window, unsigned int codepoint);
void resizeCallback(GLFWwindow* window, int width, int height);

#endif /* GraphicsWindow_hpp */
