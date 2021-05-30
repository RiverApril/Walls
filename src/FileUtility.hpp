//
//  FileUtility.hpp
//  Walls
//
//  Created by Emily Atlee on 8/15/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef FileUtility_hpp
#define FileUtility_hpp

#include "Headers.hpp"

namespace FileUtility {
    
    string readStringFile(string filename, string def);
    
    bool writeStringFile(string filename, string content);
    
    
}

#endif /* FileUtility_hpp */
