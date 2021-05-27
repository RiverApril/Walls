//
//  Shader.cpp
//  Walls
//
//  Created by Emily Atlee on 8/15/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "Shader.hpp"
#include "FileUtility.hpp"


bool Shader::loadFile(){
    code = FileUtility::readStringFile(filename, "");
    return code != "";
}

bool Shader::compile(){
    pointer = glCreateShader(type);
    const char* c = code.c_str();
    glShaderSource(pointer, 1, &c, NULL);
    glCompileShader(pointer);
    //printf("%s", c);
    
    GLint status;
    glGetShaderiv(pointer, GL_COMPILE_STATUS, &status);
    
    if (!status){
        GLint len;
        glGetShaderiv(pointer, GL_INFO_LOG_LENGTH, &len);
        GLchar* infolog = (GLchar*)malloc(sizeof(GLchar) * len);
        glGetShaderInfoLog(pointer, len, NULL, infolog);
        log = string(infolog);
    }else{
        log = "Compile Successful\n";
    }
    return status;
}



void ShaderProgram::addShader(Shader shader){
    if(shader.loadFile()){
        if(shader.compile()){
            shaders.push_back(shader.pointer);
        }
        fprintf(stderr, "%s: \n%s\n", shader.filename.c_str(), shader.log.c_str());
    }else{
        fprintf(stderr, "Failed to load file: %s\n", shader.filename.c_str());
    }
}

bool ShaderProgram::link(){
    pointer = glCreateProgram();
    for(GLuint shader : shaders){
        glAttachShader(pointer, shader);
    }
    glLinkProgram(pointer);
    GLint success;
    glGetProgramiv(pointer, GL_LINK_STATUS, &success);
    if(success == 0){
        int logLength;
        glGetProgramiv(pointer, GL_INFO_LOG_LENGTH, &logLength);
        char log[logLength];
        glGetProgramInfoLog(pointer, logLength, 0, log);
        fprintf(stderr, "Failed to link shader program: %s\n", log);
    }
    return success;
}

void ShaderProgram::use(){
    glUseProgram(pointer);
}

int ShaderProgram::getUniformLocation(string s){
    if(uniformLocations.count(s) == 0){
        int i = glGetUniformLocation(pointer, s.c_str());
        if(i == -1){
            printf("Uniform '%s' not found.\n", s.c_str());
            exit(1);
        }else{
            uniformLocations[s] = i;
        }
        return i;
    }else{
        return uniformLocations[s];
    }
}





