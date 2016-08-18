//
//  Mesh.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include "Headers.hpp"
#include "Vertex.hpp"


enum CompileStatus{
    noRender,
    needsMake,
    making,
    needsCompile,
    canRender,
    needsReset,
    needsRemoval
};

struct Mesh{
    
    CompileStatus status = noRender;
    
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    
    GLsizei elementCount;
    
    GLuint vbo;
    GLuint ibo;
    
    thread makeThread;
    
    vec3 radii;
    
    Mesh();
    virtual ~Mesh(){}
    
    void draw();
    virtual void make(){}
    void calculateRadii();
    void compile();
    void render();
    void cleanup();
    
};

void makeMesh(Mesh* mesh);

#endif /* Mesh_hpp */
