#include "Renderer.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

#include "GLutils.h"

/*******************************************************************************************************/
/* open gl drawing and state manipulation and vertex creation */
/*******************************************************************************************************/

std::vector<Vertex> generate_circle_vertices(int resolution, int screen_width, int screen_height)
{
    float width{static_cast<float>(screen_width)};
    float height{static_cast<float>(screen_height)};
    float aspect_ratio{width/height};

    std::vector<Vertex> vertices;
    vertices.reserve(resolution+2);
    vertices.push_back({0.0f, 0.0f});
    
    for(int k=0; k<resolution+1; k++)
    {
        float x1 = std::cos(2*M_PI*k / resolution);
        float y1 = aspect_ratio*std::sin(2*M_PI*k / resolution); 
        vertices.push_back({x1, y1}); 
    }

    return vertices;
}

