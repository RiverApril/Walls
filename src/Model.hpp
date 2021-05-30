//
//  Model.hpp
//  Walls
//
//  Created by Emily Atlee on 8/15/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp

#include "Headers.hpp"
#include "Mesh.hpp"

struct Model : public Mesh{
    
    string name;
    string filename;
    
    Model(string name, string filename);
    ~Model(){}
    
    virtual void make();
    
};

namespace Models{
    extern map<string, Model*> models;
    extern Model* monkey;
    extern Model* cube;
    extern Model* sphere;
    
    void initModels();
}

#endif /* Model_hpp */
