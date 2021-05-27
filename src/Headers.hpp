//
//  Headers.hpp
//  Walls
//
//  Created by Emily Atlee on 8/15/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef Headers_hpp
#define Headers_hpp

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <png.h>
#include <queue>
#include <cstdarg>

using namespace std;
using namespace std::chrono;
using namespace glm;

#define removeFromVector(vec, ele) vec.erase(std::remove(vec.begin(), vec.end(), ele), vec.end());

#define clamp(v, low, high) (v<low?low:(v>high?high:v))

#define LIGHT_COUNT_MAX 32

std::string format (const char *fmt, ...);
std::string vformat (const char *fmt, va_list ap);

#define CHECK_ERROR {GLuint error = glGetError(); if(error != GL_NO_ERROR){printf("%s:%d GL_ERROR: 0x%X\n", __FILE__, __LINE__, error); fflush(stdout); exit(1);}}

#endif /* Headers_hpp */
