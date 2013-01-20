
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

// Compile a GLSL shader file, and return the shader object.
GLuint compileShader(GLenum type, const char *filePath);
