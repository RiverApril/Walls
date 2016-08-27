//
//  Flat.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/21/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Flat_hpp
#define Flat_hpp

#include "Headers.hpp"
#include "Vertex.hpp"

struct Sprite{
    vec2 center = vec2(0, 0);
    vec2 radii = vec2(.5, .5);
    float z = 0;
    vec4 color = vec4(1, 1, 1, 1);
    vec4 UV = vec4(0, 0, 1, 1);
};

struct TextSprite{
    vec2 pos = vec2(0, 0);
    string text = "";
    float z = 0;
    bool center = false;
    vec4 color = vec4(1, 1, 1, 1);
};

class Flat{
    
public:
    vector<FlatVertex> vertices;
    vector<unsigned int> indices;
    
    GLsizei elementCount;
    
    GLuint vbo;
    GLuint ibo;
    
    bool compiled = false;
    
    vector<Sprite*> sprites;
    vector<TextSprite*> textSprites;
    
    void compile();
    void render();
    
    
};

#endif /* Flat_hpp */
