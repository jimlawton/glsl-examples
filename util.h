#include <GL/gl.h>

// Get the OpenGL version.
void getGlVersion(int *major, int *minor);

// Get the GLSL version.
void getGlslVersion(int *major, int *minor);

// Compile a GLSL shader file, and return the shader object.
GLuint compileShader(GLenum type, const char *filePath);
