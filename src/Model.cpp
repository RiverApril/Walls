//
//  Model.cpp
//  Walls
//
//  Created by Emily Atlee on 8/15/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "Model.hpp"
#include "FileUtility.hpp"


Model::Model(string name, string filename) : name(name), filename(filename){
    Models::models.insert(pair<string, Model*>(name, this));
    status = needsMake;
}


void Model::make(){
    
    printf("Making Model: %s\n", filename.c_str());
    
    ifstream ifs(filename.c_str());
    
    if (ifs) {
        
        vector<vec3> vertexList;
        vector<vec3> normalList;
        vector<vec2> textureCoordList;
        
        string line;
        
        while (getline(ifs, line)) {
            istringstream iss(line);
            string f;
            iss >> f;
            if (f == "v") { //vertex
                
                float x, y, z;
                iss >> x >> y >> z;
                
                vertexList.push_back(vec3(x, y, z));
                
            } else if (f == "vt") { //texture coords
                
                float x, y;
                iss >> x >> y;
                
                textureCoordList.push_back(vec2(x, y));
                
            } else if (f == "vn") { //normals
                
                float x, y, z;
                iss >> x >> y >> z;
                
                normalList.push_back(normalize(vec3(x, y, z)));
                
            } else if (f == "f") { //faces
                
                string fs[3];
                iss >> fs[0] >> fs[1] >> fs[2];
                    
                for (int i = 0; i < 3;i++) { // 0, 1, 2
                    bool noUV = fs[i].find("//") != std::string::npos;
                    for(int j=0;j<fs[i].size();j++){
                        if(fs[i][j] == '/'){
                            fs[i][j] = ' ';
                        }
                    }
                    istringstream fss(fs[i]);
                    
                    Vertex vert;
                    
                    if(noUV){
                        int v, n;
                        fss >> v >> n;
                        
                        vert = Vertex(vertexList[v-1], vec4(1, 1, 1, 1), normalList[n-1], vec2(0, 0));
                        
                    }else{
                        int v, t, n;
                        fss >> v >> t >> n;
                        
                        vert = Vertex(vertexList[v-1], vec4(1, 1, 1, 1), normalList[n-1], textureCoordList[t-1]);
                    }
                    
                    std::vector<Vertex>::iterator vi = find(vertices.begin(), vertices.end(), vert);
                    
                    if(vi == vertices.end()){
                        vertices.push_back(vert);
                        indices.push_back((unsigned int)vertices.size()-1);
                    }else{
                        indices.push_back((unsigned int)(vi - vertices.begin()));
                    }
                }
                
            }
        }
        
        printf("Made Model: %s\n", filename.c_str());
        return;
    }
    
    printf("Failed To Make Model: %s\n", filename.c_str());
    
}


namespace Models{
    
    map<string, Model*> models;
    
    Model* monkey;
    Model* cube;
    Model* sphere;
    
    void initModels(){
        monkey = new Model("monkey", "models/monkey.obj");
        cube = new Model("cube", "models/cube.obj");
        sphere = new Model("sphere", "models/sphere.obj");
    }
    
}

