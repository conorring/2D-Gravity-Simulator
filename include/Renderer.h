#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include <vector>

//TODO 
// 1.

struct Vertex
{
    // position
     float x;
     float y;
};

std::vector<Vertex> generate_circle_vertices(int resolution, int screen_width, int screen_height);

#endif
