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
    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    worldShader.addShader(Shader("shaders/world.vert.glsl", GL_VERTEX_SHADER));
    worldShader.addShader(Shader("shaders/world.frag.glsl", GL_FRAGMENT_SHADER));
    worldShader.link();
    
    makeProjectionMatrix();
    
    Models::initModels();
    
    
    //Setup Scene
    
    activeScene = new Scene();
    
    player = new ActorPlayer(activeScene);
    activeScene->actors.push_back(player);
    
    player->box.center.y = 10;
    
    activeScene->props.push_back(new Prop(vec3(10, .5, 10), vec3(0, -.25, 0)));
    //activeScene->props.push_back(new Prop(vec3(.25, .25, .25), vec3(0, 1, 0)));
    //activeScene->props.push_back(new Prop(vec3(.5, .5, .5), vec3(0, 1, 1)));
    //activeScene->props.push_back(new Prop(Models::monkey, vec3(1, 1, 0)));
    //
    
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    frames = 0;
    startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    passedTime = 0;
    
    
    return true;
    
}

void GraphicsWindow::makeProjectionMatrix(){
    matrixProjection = perspectiveFov<float>(radians(80.0f), settings->windowSize.x, settings->windowSize.y, 0.1f, 1000);
}

void GraphicsWindow::startLoop(){
    while(running && !glfwWindowShouldClose(window)){
        tick++;
        
        //FPS
        frames ++;
        passedTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if(passedTime > startTime + 1000){
            glfwSetWindowTitle(window, ("FPS: "+to_string(frames)).c_str());
            startTime = passedTime;
            frames = 0;
        }
        //
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //user input
        mousePosPrev = mousePos;
        glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE){
            mousePosDelta = mousePos - mousePosPrev;
        }else{
            mousePosDelta = dvec2();
        }
        //
        
        
        //start using world shader
        worldShader.use();
        
        //set camera
        matrixCamera = mat4();
        matrixCamera = rotate<float>(matrixCamera, cameraRotation.z, vec3(0, 0, 1.0f));
        matrixCamera = rotate<float>(matrixCamera, cameraRotation.x, vec3(1.0f, 0, 0));
        matrixCamera = rotate<float>(matrixCamera, cameraRotation.y, vec3(0, 1.0f, 0));
        matrixCamera = translate(matrixCamera, -cameraPosition);
        //
        
        //Render scene
        activeScene->render(this);
        
        float yaw = cameraRotation.y;
        float pitch = cameraRotation.x;
        
        /*
         
        mine:
        (pitch, yaw ) -> (x,  y,  z)
        (0    ,  0  ) -> (0,  0, -1)
        (pi/2 ,  0  ) -> (0, -1,  0)
        (0    , pi/2) -> (1,  0,  0)

        should be:
        (pitch, yaw)  -> (x, y, z)
        (0,     0)    -> (1, 0, 0)
        (pi/2,  0)    -> (0, 1, 0)
        (0,    -pi/2) -> (0, 0, 1)

        original formula:
        x = cos(yaw) * cos(pitch)
        y = sin(pitch)
        z = sin(-yaw) * cos(pitch)

        So I swapped the x and z, and negated y and z

        */
        cameraLook = normalize(vec3(sin(yaw)*cos(pitch), -sin(pitch), -cos(yaw)*cos(pitch)));
        
        if(propToPlace){
            RayData ray = activeScene->rayProps(cameraPosition, cameraLook);
            rayHit = ray.hit;
            if(ray.hit){
                vec3 newPos = ray.prop->box.center + ((ray.prop->box.radii + propToPlace->box.radii) * sideNormal(ray.side));
                newPos = (ray.position * sideInvMask(ray.side)) + (newPos * sideMask(ray.side));
                newPos = roundTo(newPos, 4);
                propToPlace->box.center = newPos;
                
                bool collision = false;
                for(Prop* prop : activeScene->props){
                    if(prop->box.intersects(propToPlace->box)){
                        collision = true;
                        break;
                    }
                }
                
                //glDisable(GL_DEPTH_TEST);
                glUniform1i(worldShader.getUniformLocation("overrideColor"), 1);
                if(collision){
                    glUniform4f(worldShader.getUniformLocation("colorOverride"), 1, 0, 0, .5);
                }else{
                    glUniform4f(worldShader.getUniformLocation("colorOverride"), 0, 1, 0, .5);
                }
                propToPlace->render(this);
                glUniform1i(worldShader.getUniformLocation("overrideColor"), 0);
                //glEnable(GL_DEPTH_TEST);
                
            }
            
        }
        
        
        
        glfwPollEvents();
        glfwSwapBuffers(window);
        
        GLenum error = glGetError();
        if(error != 0){
            printf("Uh oh, we've got a GL error somewhere: 0x%X\n", error);
        }
    }
}

void GraphicsWindow::keyEvent(int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE){
        if(action == GLFW_PRESS){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }else if(action == GLFW_RELEASE){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_1){
            if(propToPlace){
                delete propToPlace;
                propToPlace = nullptr;
            }
            propToPlace = new Prop(vec3(.25, .25, .25), vec3(0, 0, 0));
        }else if(key == GLFW_KEY_2){
            if(propToPlace && rayHit){
                activeScene->props.push_back(propToPlace);
                propToPlace = nullptr;
            }
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(window == graphicsWindowInstance->window){
        graphicsWindowInstance->keyEvent(key, scancode, action, mods);
    }else{
        printf("Another window?\n");
    }
    
}

void resizeCallback(GLFWwindow* window, int width, int height){
    if(window == graphicsWindowInstance->window){
        graphicsWindowInstance->settings->windowSize.x = width;
        graphicsWindowInstance->settings->windowSize.y = height;
        graphicsWindowInstance->makeProjectionMatrix();
    }else{
        printf("Another window?\n");
    }
}





