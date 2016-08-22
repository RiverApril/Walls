//
//  GraphicsWindow.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "GraphicsWindow.hpp"
#include "Texture.hpp"

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
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    worldShader.addShader(Shader("shaders/world.vert.glsl", GL_VERTEX_SHADER));
    worldShader.addShader(Shader("shaders/world.frag.glsl", GL_FRAGMENT_SHADER));
    worldShader.link();
    
    guiShader.addShader(Shader("shaders/gui.vert.glsl", GL_VERTEX_SHADER));
    guiShader.addShader(Shader("shaders/gui.frag.glsl", GL_FRAGMENT_SHADER));
    guiShader.link();
    
    makeProjectionMatrix();
    
    Models::initModels();
    Textures::initTextures();
    
    
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
    
    gui = new Gui();
    
    Sprite* redical = new Sprite();
    redical->center = vec2(0, 0);
    redical->radii = vec2(4, 4);
    redical->UV = vec4(0, 1.0f/32*6, 1.0f/32, 1.0f/32*7);
    redical->color = vec4(1, 1, 1, 1);
    gui->sprites.push_back(redical);
    
    consoleSprite = new TextSprite();
    gui->textSprites.push_back(consoleSprite);
    
    gui->compile();
    
    
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
    matrixProjection = perspectiveFov<float>(radians(80.0f), settings->windowSize.x, settings->windowSize.y, 0.001f, 1000);
    matrixGui = mat4();
    matrixGui = scale(matrixGui, vec3(2.0f/settings->windowSize.x, -2.0f/settings->windowSize.y, 1.0f));
}

void GraphicsWindow::startLoop(){
    while(running && !glfwWindowShouldClose(window)){
        tick++;
        
        //FPS
        frames ++;
        passedTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if(passedTime > startTime + 1000){
            fps = frames;
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
        
        
        glCullFace(GL_BACK);
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
        
        okayToPlace = false;
        selectedProp = nullptr;
        if(propToPlace){
            RayData ray = activeScene->rayProps(cameraPosition, cameraLook);
            if(ray.hit){
                okayToPlace = true;
                vec3 newPos = ray.prop->box.center + ((ray.prop->box.radii + propToPlace->box.radii) * sideNormal(ray.side));
                newPos = (ray.position * sideInvMask(ray.side)) + (newPos * sideMask(ray.side));
                newPos = roundTo(newPos, 8);
                propToPlace->box.center = newPos;
                
                bool collision = false;
                for(Prop* prop : activeScene->props){
                    if(prop->box.intersects(propToPlace->box)){
                        collision = true;
                        break;
                    }
                }
                
                glUniform1i(worldShader.getUniformLocation("overrideColor"), 1);
                if(collision){
                    okayToPlace = false;
                    glUniform4f(worldShader.getUniformLocation("colorOverride"), 1, 0, 0, .5);
                }else{
                    glUniform4f(worldShader.getUniformLocation("colorOverride"), 0, 1, 0, .5);
                }
                propToPlace->render(this);
                glUniform1i(worldShader.getUniformLocation("overrideColor"), 0);
                
            }
            
        }else{
            RayData ray = activeScene->rayProps(cameraPosition, cameraLook);
            if(ray.hit){
                glDisable(GL_DEPTH_TEST);
                glUniform1i(worldShader.getUniformLocation("overrideColor"), 1);
                glUniform4f(worldShader.getUniformLocation("colorOverride"), 1, 1, 0, .5);
                ray.prop->render(this);
                glUniform1i(worldShader.getUniformLocation("overrideColor"), 0);
                glEnable(GL_DEPTH_TEST);
                selectedProp = ray.prop;
            }
        }
        
        //Gui
        
        consoleSprite->pos = vec2(-settings->windowSize.x/2, -settings->windowSize.y/2);
        consoleSprite->text = "FPS: "+to_string(fps);
        if(consoleActive){
            consoleSprite->text += "\n\n";
            for(int i=0;i<consoleOutput.size();i++){
                consoleSprite->text += consoleOutput[i] + "\n";
            }
            consoleSprite->text += "\n>";
            consoleSprite->text += consoleInput;
            consoleSprite->text += tick%60>=30?"_":" ";
        }
        gui->compile();
        
        glCullFace(GL_FRONT);
        guiShader.use();
        glUniform1i(guiShader.getUniformLocation("activeTexture"), 0);
        glUniformMatrix4fv(guiShader.getUniformLocation("viewMatrix"), 1, false, &matrixGui[0][0]);
        gui->render();
        //
        
        glfwPollEvents();
        glfwSwapBuffers(window);
        
        GLenum error = glGetError();
        if(error != 0){
            printf("Uh oh, we've got a GL error somewhere: 0x%X\n", error);
        }
    }
}

void GraphicsWindow::charEvent(int codepoint){
    if(consoleActive){
        if(codepoint >= 32 && codepoint <= 127 && codepoint != '`'){
            consoleInput += codepoint;
        }
    }
}

void GraphicsWindow::proccessCommand(string command){
    stringstream ss(command);
    string name;
    ss >> name;
    if(name == "tp"){//tp x y z
        float x, y, z;
        ss >> x >> y >> z;
        player->box.center = vec3(x, y, z);
    }else if(name == "clear"){
        consoleOutput.clear();
    }
}

void GraphicsWindow::consoleAdd(string add){
    consoleOutput.push_back(add);
    if(consoleOutput.size() > (settings->windowSize.y / 8)-6){
        consoleOutput.erase(consoleOutput.begin());
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
    
    if(consoleActive){
        if(action == GLFW_PRESS){
            if(key == GLFW_KEY_GRAVE_ACCENT){
                consoleActive = false;
            }else if(key == GLFW_KEY_BACKSPACE){
                consoleInput = consoleInput.substr(0, consoleInput.size()-1);
            }else if(key == GLFW_KEY_ENTER){
                consoleAdd(consoleInput);
                proccessCommand(consoleInput);
                consoleInput = "";
            }
        }
    }else{
        if(action == GLFW_PRESS){
            if(key == GLFW_KEY_GRAVE_ACCENT){
                consoleActive = true;
            }else if(key == GLFW_KEY_1){
                if(propToPlace){
                    delete propToPlace;
                    propToPlace = nullptr;
                }
                propToPlace = new Prop(vec3(.5, .5, .5), vec3(0, 0, 0));
            }else if(key == GLFW_KEY_2){
                if(propToPlace){
                    delete propToPlace;
                    propToPlace = nullptr;
                }
                propToPlace = new Prop(vec3(.25, .25, .25), vec3(0, 0, 0));
            }else if(key == GLFW_KEY_3){
                if(propToPlace){
                    delete propToPlace;
                    propToPlace = nullptr;
                }
                propToPlace = new Prop(vec3(.125, .125, .125), vec3(0, 0, 0));
            }else if(key == GLFW_KEY_ENTER){
                if(propToPlace && okayToPlace){
                    activeScene->props.push_back(propToPlace);
                    propToPlace = nullptr;
                }
            }else if(key == GLFW_KEY_BACKSPACE){
                if(selectedProp){
                    removeFromVector(activeScene->props, selectedProp);
                    selectedProp = nullptr;
                }
            }
        }
    }
}

void GraphicsWindow::mouseButtonEvent(int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_1){
        if(propToPlace && okayToPlace){
            activeScene->props.push_back(propToPlace);
            propToPlace = nullptr;
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

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    if(window == graphicsWindowInstance->window){
        graphicsWindowInstance->mouseButtonEvent(button, action, mods);
    }else{
        printf("Another window?\n");
    }
}

void charCallback(GLFWwindow* window, unsigned int codepoint){
    if(window == graphicsWindowInstance->window){
        graphicsWindowInstance->charEvent(codepoint);
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





