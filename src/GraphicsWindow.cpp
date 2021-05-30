//
//  GraphicsWindow.cpp
//  Walls
//
//  Created by Emily Atlee on 8/15/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "GraphicsWindow.hpp"
#include "Texture.hpp"
#include "FileUtility.hpp"

GraphicsWindow* graphicsWindowInstance;

bool GraphicsWindow::initWindow(){
    
    if(!glfwInit()){
        fprintf(stderr, "ERROR: Failed to start GLFW\n");
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(settings->windowSize.x, settings->windowSize.y, title.c_str(), NULL, NULL);
    
    if(!window){
        fprintf(stderr, "ERROR: Failed to create window with GLFW\n");
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    // gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
       fprintf(stderr, "ERROR: Failed to start GLEW\n");
       return false;
    }
    
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported: %s\n", version);
    printf("GLFW Version: %s\n", glfwGetVersionString());
    printf("\n");
    
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    worldShader.addShader(Shader("shaders/world.vert.glsl", GL_VERTEX_SHADER));
    worldShader.addShader(Shader("shaders/world.frag.glsl", GL_FRAGMENT_SHADER));
    worldShader.link();
    
    flatShader.addShader(Shader("shaders/flat.vert.glsl", GL_VERTEX_SHADER));
    flatShader.addShader(Shader("shaders/flat.frag.glsl", GL_FRAGMENT_SHADER));
    flatShader.link();
    
    shadowShader.addShader(Shader("shaders/shadow.vert.glsl", GL_VERTEX_SHADER));
    shadowShader.addShader(Shader("shaders/shadow.geom.glsl", GL_GEOMETRY_SHADER));
    shadowShader.addShader(Shader("shaders/shadow.frag.glsl", GL_FRAGMENT_SHADER));
    shadowShader.link();
    
    makeProjectionMatrix();
    makeShadowBuffers();
    
    Models::initModels();
    Textures::initTextures();
    Materials::initMaterials();
    
    //Setup Scene
    
    activeScene = new Scene();
    
    ActorPlayer* player = new ActorPlayer(activeScene);
    activeScene->addActor(player);
    
    player->box.center.y = 10;
    
    activeScene->addProp(new Prop(vec3(10, .5, 10), vec3(0, -0.5, 0)));
    // activeScene->addProp(new Prop(vec3(10, .5, 10), vec3(0, -15, 0)));
    // activeScene->addProp(new Prop(vec3(10, .5, 10), vec3(0, 15, 0)));
    // activeScene->addProp(new Prop(vec3(.5, 10, 10), vec3(-15, 0, 0)));
    // activeScene->addProp(new Prop(vec3(.5, 10, 10), vec3(15, 0, 0)));
    // activeScene->addProp(new Prop(vec3(10, 10, .5), vec3(0, 0, -15)));
    // activeScene->addProp(new Prop(vec3(10, 10, .5), vec3(0, 0, 15)));

    // activeScene->addProp(new Prop(vec3(1, 1, 1), vec3(1, 0, 3)));
    // activeScene->addProp(new Prop(vec3(.5, .5, .5), vec3(0, 5, 1)));
    // activeScene->addProp(new Prop(vec3(0.25, 0.25, 0.25), vec3(3, 1, 0)));

    // PointLight* lamp = new PointLight();
    // activeScene->addProp(new Prop(vec3(.1, .1, .1), vec3(0, 1, 0)));
    // lamp->position = vec3(0, 1, 0);
    // lamp->specular = vec3(1.0, 1.0, 1.0);
    // lamp->attenuation = vec3(0, 1, 0);
    // activeScene->addPointLight(lamp);
    activeScene->addProp(new Prop(vec3(.25, .25, .25), vec3(0, 1, 0)));
    activeScene->addProp(new Prop(vec3(.5, .5, .5), vec3(0, 1, 1)));
    activeScene->addProp(new Prop(Models::monkey, Materials::defaultMaterial, vec3(1, 1, 0)));
    //
    
    //setup hud
    hud = new Flat();
    
    Sprite* redical = new Sprite();
    redical->center = vec2(0, 0);
    redical->radii = vec2(4, 4);
    redical->UV = vec4(0, 1.0f/32*6, 1.0f/32, 1.0f/32*7);
    redical->color = vec4(1, 1, 1, 1);
    hud->sprites.push_back(redical);
    
    consoleSprite = new TextSprite();
    hud->textSprites.push_back(consoleSprite);
    
    hud->compile();
    //
    
    //setup hologram
    testHologram = new Hologram();
    testHologram->pos = vec3(0, 1.0f, 0);
    
    Flat* holoFlat = new Flat();
    testHologram->flat = holoFlat;
    testHologram->lockOnPlayer = true;
    
    TextSprite* holoWord = new TextSprite();
    holoWord->text = "Hologram\nNew Line\n\nSpace";
    holoWord->center = true;
    
    holoFlat->textSprites.push_back(holoWord);
    
    holoFlat->compile();
    //
    
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.0, 0.0, 1.0, 1.0);
    
    frames = 0;
    startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    passedTime = 0;
    
    
    GLenum error = glGetError();
    if(error != 0){
        printf("Uh oh, we've got a GL error somewhere: 0x%X\n", error);
        return false;
    }
    
    return true;
    
}

void GraphicsWindow::makeProjectionMatrix(){
    matrixProjection = perspectiveFov<float>(radians(settings->fov), settings->windowSize.x, settings->windowSize.y, 0.001f, 1000);
    matrixHud = identity<mat4>();
    matrixHud = scale(matrixHud, vec3(2.0f/settings->windowSize.x, -2.0f/settings->windowSize.y, 1.0f));
}

void GraphicsWindow::makeShadowBuffers(){

    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, depthCubemap);
    glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 1, GL_DEPTH_COMPONENT24, settings->shadowResolution, settings->shadowResolution, 6 * LIGHT_COUNT_MAX);

    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);

    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    for(int i = 0; i < LIGHT_COUNT_MAX; i++){
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicsWindow::startLoop(){
    while(running && !glfwWindowShouldClose(window)){
        tick++;
        
        //FPS
        frames ++;
        passedTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if(passedTime > startTime + 1000){
            fps = frames;
            startTime = passedTime;
            frames = 0;
        }
        //

        renderShadows();

        // render scene with shadow mapping
        glViewport(0, 0, settings->windowSize.x*2, settings->windowSize.y*2);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderScene();
        //

        renderHud();
        
        glfwSwapBuffers(window);
        glfwPollEvents();

        CHECK_ERROR
    }
}

void GraphicsWindow::renderShadows(){
    activeScene->renderShadows(this);
}

void GraphicsWindow::renderScene(){
    
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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Textures::prop->pointer);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Textures::flat->pointer);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, depthCubemap);
    
    //set camera
    matrixCamera = identity<mat4>();
    matrixCamera = rotate<float>(matrixCamera, cameraRotation.z, vec3(0, 0, 1.0f));
    matrixCamera = rotate<float>(matrixCamera, cameraRotation.x, vec3(1.0f, 0, 0));
    matrixCamera = rotate<float>(matrixCamera, cameraRotation.y, vec3(0, 1.0f, 0));
    matrixCamera = translate(matrixCamera, -cameraPosition);

    glUniformMatrix4fv(worldShader.getUniformLocation("viewMatrix"), 1, false, &matrixCamera[0][0]);
    glUniformMatrix4fv(worldShader.getUniformLocation("projectionMatrix"), 1, false, &matrixProjection[0][0]);
    //
    
    //Render scene
    glUniform3f(worldShader.getUniformLocation("eye"), cameraPosition.x, cameraPosition.y, cameraPosition.z);

    glUniform1i(worldShader.getUniformLocation("depthMap"), 2);

    activeScene->render(this);
    
    float yaw = cameraRotation.y;
    float pitch = cameraRotation.x;
    
    
        
//        mine:
//        (pitch, yaw ) -> (x,  y,  z)
//        (0    ,  0  ) -> (0,  0, -1)
//        (pi/2 ,  0  ) -> (0, -1,  0)
//        (0    , pi/2) -> (1,  0,  0)
//
//        should be:
//        (pitch, yaw)  -> (x, y, z)
//        (0,     0)    -> (1, 0, 0)
//        (pi/2,  0)    -> (0, 1, 0)
//        (0,    -pi/2) -> (0, 0, 1)
//
//        original formula:
//        x = cos(yaw) * cos(pitch)
//        y = sin(pitch)
//        z = sin(-yaw) * cos(pitch)
//
//        So I swapped the x and z, and negated y and z

    
    cameraLook = normalize(vec3(sin(yaw)*cos(pitch), -sin(pitch), -cos(yaw)*cos(pitch)));
    
    okayToPlace = false;
    selectedProp = nullptr;
    lookRay = activeScene->rayProps(cameraPosition, cameraLook);
    if(lookRay.hit){
        if(propToPlace){
            okayToPlace = true;
            vec3 newPos = lookRay.prop->box.center + ((lookRay.prop->box.radii + propToPlace->box.radii) * sideNormal(lookRay.side));
            newPos = (lookRay.position * sideInvMask(lookRay.side)) + (newPos * sideMask(lookRay.side));
            newPos = roundTo(newPos, 8);
            propToPlace->box.center = newPos;
            
            bool collision = false;
            auto props = activeScene->getProps();
            for(Prop* prop : props){
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
            
        }else{
            selectedProp = lookRay.prop;
        }
    }
}

void GraphicsWindow::renderHud(){
    //Flat
    consoleSprite->pos = vec2(-settings->windowSize.x/2, -settings->windowSize.y/2);
    consoleSprite->text = format("FPS %d", fps);
    if(debugActive){
        ActorPlayer* player = activeScene->getPlayer();
        consoleSprite->text += format("\nPos %0.2f %0.2f %0.2f", player->box.center.x, player->box.center.y, player->box.center.z);
        consoleSprite->text += format("\nEye %0.2f %0.2f %0.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
        consoleSprite->text += format("\nLook %s %s %s", cameraLook.x>0?"x+":"x-", cameraLook.y>0?"y+":"y-", cameraLook.z>0?"z+":"z-");
        consoleSprite->text += format("\nYaw %0.2f  Pitch %0.2f", degrees(cameraRotation.y), degrees(cameraRotation.x));
    }
    if(consoleActive){
        consoleSprite->text += "\n\n";
        for(int i=0;i<consoleOutput.size();i++){
            consoleSprite->text += consoleOutput[i] + "\n";
        }
        consoleSprite->text += "\n>";
        consoleSprite->text += consoleInput;
        consoleSprite->text += tick%60>=30?"_":" ";
    }
    hud->compile();
    
    flatShader.use();
    glUniform1i(flatShader.getUniformLocation("activeTexture"), 1);
    glUniformMatrix4fv(flatShader.getUniformLocation("viewMatrix"), 1, false, &matrixHud[0][0]);
    hud->render();
    //
    
    testHologram->render(this);
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
    if (name == "tp") { // tp x y z
        float x, y, z;
        ss >> x >> y >> z;
        ActorPlayer* player = activeScene->getPlayer();
        player->box.center = vec3(x, y, z);
    } else if(name == "clear") {
        consoleOutput.clear();
    } else if(name == "save") { // save scenefile
        string filename;
        ss >> filename;

        string sceneData = activeScene->save();
        bool success = FileUtility::writeStringFile(filename, sceneData);
        if (success) {
            consoleAdd("Saved scene to file: " + filename);
        } else {
            consoleAdd("Failed to save scene to file: " + filename);
        }
    } else if(name == "load") { // load scenefile
        string filename;
        ss >> filename;

        string sceneData = FileUtility::readStringFile(filename, "");
        if(sceneData == ""){
            consoleAdd("Failed to load scene from file: " + filename);
        }
        stringstream sceneStream(sceneData);
        Scene* newScene = loadScene(sceneStream);
        if(newScene){
            activeScene = newScene;
            consoleAdd("Loaded scene from file: " + filename);
        } else {
            consoleAdd("Invalid scene in file: " + filename);
        }
    }
}

void GraphicsWindow::consoleAdd(string add){
    consoleOutput.push_back(add);
    if(consoleOutput.size() > (settings->windowSize.y / 10)-10){
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
            if (key == GLFW_KEY_GRAVE_ACCENT) {
                consoleActive = false;
            } else if(key == GLFW_KEY_BACKSPACE){
                consoleInput = consoleInput.substr(0, consoleInput.size()-1);
            } else if(key == GLFW_KEY_ENTER){
                if(consoleInput != ""){
                    consoleAdd(consoleInput);
                    proccessCommand(consoleInput);
                    consoleInput = "";
                }else{
                    consoleActive = false;
                }
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
            }else if(key == GLFW_KEY_4){
                if(lookRay.hit){
                    PointLight* lamp = new PointLight();
                    lamp->position = lookRay.position + vec3(0, 0.1, 0);
                    lamp->specular = vec3(1.0, 1.0, 1.0);
                    lamp->diffuse = vec3(1.0, 1.0, 1.0);
                    lamp->attenuation = vec3(0, 0.5, 1);
                    activeScene->addPointLight(lamp);
                }
            }else if(key == GLFW_KEY_5){
                if(propToPlace){
                    delete propToPlace;
                    propToPlace = nullptr;
                }
                propToPlace = new Prop(Models::sphere, Materials::defaultMaterial, vec3(0, 0, 0));
            }else if(key == GLFW_KEY_ENTER){
                if(propToPlace && okayToPlace){
                    activeScene->addProp(propToPlace);
                    propToPlace = nullptr;
                }
            }else if(key == GLFW_KEY_BACKSPACE){
                if(propToPlace){
                    delete propToPlace;
                    propToPlace = nullptr;
                }else if(selectedProp){
                    activeScene->removeProp(selectedProp);
                    selectedProp = nullptr;
                }
            }else if(key == GLFW_KEY_F1){
                debugActive = !debugActive;
            }
        }
    }
}

void GraphicsWindow::mouseButtonEvent(int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_1){
        //printf("Place?\n");
        if(propToPlace && okayToPlace){
            activeScene->addProp(propToPlace);
            propToPlace = nullptr;
            //printf("Placed\n");
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




