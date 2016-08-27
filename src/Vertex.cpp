//
//  Vertex.cpp
//  Walls
//
//  Created by Braeden Atlee on 8/15/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "Vertex.hpp"

bool operator!=(const Vertex& a, const Vertex& b){
    return (a.Position != b.Position) || (a.Color != b.Color) || (a.Normal != b.Normal) || (a.UV != b.UV);
}

bool operator==(const Vertex& a, const Vertex& b){
    return !(a!=b);
}

bool operator!=(const FlatVertex& a, const FlatVertex& b){
    return (a.Position != b.Position) || (a.Color != b.Color) || (a.UV != b.UV);
}

bool operator==(const FlatVertex& a, const FlatVertex& b){
    return !(a!=b);
}