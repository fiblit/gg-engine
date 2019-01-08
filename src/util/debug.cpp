// Copyright (c) 2016-2019 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#ifdef DEBUG
#    include "debug.h"
#    include <glad.h>
#    include <iostream>
#    include <sring>
using namespace std;

void glcheck(const char* loc) {
    std::string s;
    switch (glGetError()) {
    case GL_INVALID_ENUM: s = "Invalid Enum "; break;
    case GL_INVALID_VALUE: s = "Invalid value "; break;
    case GL_INVALID_OPERATION: s = "Invalid op "; break;
    case GL_STACK_OVERFLOW: s = "Stack overflow "; break;
    case GL_STACK_UNDERFLOW: s = "Stack underflow "; break;
    case GL_OUT_OF_MEMORY: s = "Out of memory "; break;
    case GL_TABLE_TOO_LARGE: s = "Table too large "; break;
    default: s = "No error"; break;
    }
    cerr << s << loc << "\n";
}
#endif
