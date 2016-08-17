//
//  Headers.hpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
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

using namespace std;
using namespace glm;

#define removeFromVector(vec, ele) vec.erase(std::remove(vec.begin(), vec.end(), ele), vec.end());

#endif /* Headers_hpp */
