//
//  Vertex.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Vertex.hpp"

bool operator!=(const Vertex& a, const Vertex& b){
    return (a.Position != b.Position) || (a.Color != b.Color) || (a.Normal != b.Normal);
}

bool operator==(const Vertex& a, const Vertex& b){
    return !(a!=b);
}

bool operator!=(const GuiVertex& a, const GuiVertex& b){
    return (a.Position != b.Position) || (a.Color != b.Color) || (a.UV != b.UV);
}

bool operator==(const GuiVertex& a, const GuiVertex& b){
    return !(a!=b);
}