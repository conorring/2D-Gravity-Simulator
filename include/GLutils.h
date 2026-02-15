#ifndef GLUTILS_H
#define GLUTILS_H

#include <GL/glew.h>

#include <csignal>
#include <iostream>

//TODO 
// 1. 

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

/// Takes a symbolic type constant and returns its size
/// Probably missing a few types but those were the most important
//
// Function below kindly borrowed from
// Ref: https://gist.github.com/davawen/af1490ffb3bbcf9ddc0cbab82e9f27aa
// wasn't bothered typing it myself
GLsizei get_sizeof_type(GLenum type);
#endif
