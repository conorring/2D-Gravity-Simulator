#include "GLutils.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL Error (" << error << ')' << '\n' <<
            "Function call: " << function << '\n' << file << ": " << line << std::endl;
        return false;
    }
    return true;
}

/// Takes a symbolic type constant and returns its size
/// Probably missing a few types but those were the most important
//
// Function below kindly borrowed from
// wasn't bothered typing it myself
// Ref: https://gist.github.com/davawen/af1490ffb3bbcf9ddc0cbab82e9f27aa
GLsizei get_sizeof_type(GLenum type)
{
	switch(type)
	{
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			return sizeof(GLbyte);
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
			return sizeof(GLshort);
		case GL_INT_2_10_10_10_REV:
		case GL_INT:
		case GL_UNSIGNED_INT_2_10_10_10_REV:
		case GL_UNSIGNED_INT:
			return sizeof(GLint);
		case GL_FLOAT:
			return sizeof(GLfloat);
		case GL_DOUBLE:
			return sizeof(GLdouble);
		case GL_FIXED:
			return sizeof(GLfixed);
		case GL_HALF_FLOAT:
			return sizeof(GLhalf);
	}

	return 0;
}
