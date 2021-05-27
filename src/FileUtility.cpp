//
//  FileUtility.cpp
//  Walls
//
//  Created by Emily Atlee on 8/15/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "FileUtility.hpp"


namespace FileUtility{
    
    
    string readStringFile(string filename, string def){
        try{
            ifstream ifs(filename.c_str());
            string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
            return content;
        }catch(ifstream::failure e){
            return def;
        }
    }
    
    bool writeStringFile(string filename, string content){
        try{
            ofstream ofs(filename.c_str());
            ofs << content;
            return true;
        }catch(ofstream::failure e){
            return false;
        }
    }
    
    
}