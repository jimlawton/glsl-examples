
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

#include "util.h"

static const int WINDOW_WIDTH = 400;
static const int WINDOW_HEIGHT = 300;

static int window;

void usage(const char *progname)
{
    fprintf(stderr, "Usage: %s TYPE filename\n\n", progname);
    fprintf(stderr, "TYPE: -v   Compile a Vertex Shader.\n");
    fprintf(stderr, "      -f   Compile a Fragment Shader.\n");
    exit (8);
}

int main(int argc, char *argv[])
{
    int major, minor;
    GLuint program;
    GLint result;
    GLuint shader;
    GLenum type;
    const char *srcfile;
    const char *progname = argv[0];

    if (argc == 1 || argc <3) {
        usage(progname);
    } else {
        while ((argc > 1) && (argv[1][0] == '-')) {
            switch (argv[1][1]) {
            case 'v':
                type = GL_VERTEX_SHADER;
                srcfile = argv[2];
                break;
            case 'f':
                type = GL_FRAGMENT_SHADER;
                srcfile = argv[2];
                break;
            default:
                usage(progname);
                break;
            }
            ++argv;
            --argc;
        }
    }

    printf("Compiling %s...\n", srcfile);

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    window = glutCreateWindow("OpenGL Shading Demo");

    getGlVersion(&major, &minor);
    getGlslVersion(&major, &minor);

    program = glCreateProgram();

    shader = compileShader(type, srcfile);
    if (shader != 0) {
        glAttachShader(program, shader);
        // Mark the shader for deletion when the program is destroyed.
        glDeleteShader(shader);
    } else {
        fprintf(stderr, "Error compiling shader %s\n", srcfile);
    }

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        GLint length;
        char *log;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        log = malloc(length);
        glGetProgramInfoLog(program, length, &result, log);

        fprintf(stderr, "sceneInit(): Program linking failed: %s\n", log);
        free(log);

        glDeleteProgram(program);
        program = 0;
    }

    return 0;
}
