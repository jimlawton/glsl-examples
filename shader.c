
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

#include "shader.h"

// Read a vertex/fragment shader source file, and return it as a string.
static char *readShaderSource(const char *path)
{
    const size_t blockSize = 512;
    FILE *fp;
    char buf[blockSize];
    char *source = NULL;
    size_t tmp, srclen = 0;

    fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "ERROR: unable to open file %s\n", path);
        return NULL;
    }

    while ((tmp = fread(buf, 1, blockSize, fp)) > 0) {
        char *newSource = malloc(srclen + tmp + 1);
        if (!newSource) {
            fprintf(stderr, "ERROR: malloc failed!\n");
            if (source)
                free(source);
            return NULL;
        }

        if (source) {
            memcpy(newSource, source, srclen);
            free(source);
        }
        memcpy(newSource + srclen, buf, tmp);

        source = newSource;
        srclen += tmp;
    }

    fclose(fp);
    if (source)
        source[srclen] = '\0';

    return source;
}

// Compile a GLSL shader file, and return the shader object.
GLuint compileShader(GLenum type, const char *path)
{
    char *source;
    GLuint shader;
    GLint length, result;

    source = readShaderSource(path);
    if (!source)
        return 0;

    shader = glCreateShader(type);
    length = strlen(source);
    glShaderSource(shader, 1, (const char **)&source, &length);
    glCompileShader(shader);
    free(source);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        char *log;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        log = malloc(length);
        glGetShaderInfoLog(shader, length, &result, log);
        fprintf(stderr, "ERROR: unable to compile %s!\nLog:\n", path);
        fprintf(stderr, "%s\n", log);
        free(log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}
