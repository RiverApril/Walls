//
//  Model.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Model.hpp"
#include "FileUtility.hpp"


Model::Model(string filename) : filename(filename){
    status = needsMake;
}


void Model::make(){
    
    /*vertices.push_back(Vertex(vec3(0, 0.5f, 0), vec4(0, 1, 1, 1), vec3(0, 0, 0), vec3(0, 0, 0)));
    vertices.push_back(Vertex(vec3(0.5f, -0.5f, 0), vec4(1, 1, 0, 1), vec3(0, 0, 0), vec3(0, 0, 0)));
    vertices.push_back(Vertex(vec3(-0.5f, -0.5f, 0), vec4(1, 0, 1, 1), vec3(0, 0, 0), vec3(0, 0, 0)));
    
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);*/
    
    printf("Making Model: %s\n", filename.c_str());
    
    uint nextI = 0;
    
    ifstream ifs(filename.c_str());
    
    if (ifs) {
        
        vector<vec3> vertexList;
        vector<vec3> textureCoordList;
        vector<vec3> normalList;
        
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
                
                textureCoordList.push_back(vec3(x, y, 0));
                
            } else if (f == "vn") { //normals
                
                float x, y, z;
                iss >> x >> y >> z;
                
                normalList.push_back(normalize(vec3(x, y, z)));
                
            } else if (f == "f") { //faces
                
                string fs[3];
                iss >> fs[0] >> fs[1] >> fs[2];
                
                for (int i = 0; i < 3;i++) {//0, 1, 2
                    for(int j=0;j<fs[i].size();j++){
                        if(fs[i][j] == '/'){
                            fs[i][j] = ' ';
                        }
                    }
                    istringstream fss(fs[i]);
                    
                    int v, t, n;
                    fss >> v >> t >> n;
                    
                    vertices.push_back(Vertex(vertexList[v], vec4(1, 1, 1, 1), textureCoordList[t], normalList[n]));
                    indices.push_back(nextI);
                    nextI++;
                    
                }
                
            }
        }
        
        printf("Made Model: %s\n", filename.c_str());
        return;
    }
    
    printf("Failed To Make Model: %s\n", filename.c_str());
    
    
    
}