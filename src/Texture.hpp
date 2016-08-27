//
//  Texture.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/21/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include "Headers.hpp"

class Texture{
public:
    Texture(string filename, int index);
    
    GLuint pointer;
    int index;
    int width;
    int height;
    
    
    
};

namespace Textures{
    extern Texture* flat;
    extern Texture* prop;
    
    void initTextures();
}

GLuint png_texture_load(const char * file_name, int * width, int * height);

#endif /* Texture_hpp */
