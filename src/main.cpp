//
//  main.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Settings.hpp"
#include "GraphicsWindow.hpp"

int main(int argc, const char * argv[]) {
    
    Settings* settings = new Settings();
    settings->load();
    
    graphicsWindowInstance = new GraphicsWindow(settings, "Walls");
    
    graphicsWindowInstance->startLoop();
    
    settings->save();
    
    delete graphicsWindowInstance;
    delete settings;
    
    
    return 0;
}
