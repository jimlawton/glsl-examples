
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

#include "util.h"

void getGlVersion(int *major, int *minor)
{
    const char *verstr = (const char *)glGetString(GL_VERSION);

    if (!verstr || (sscanf(verstr, "%d.%d", major, minor) != 2)) {
        *major = *minor = 0;
        fprintf(stderr, "ERROR: invalid GL_VERSION format!\n");
    }
}

void getGlslVersion(int *major, int *minor)
{
    int glMajor, glMinor;

    *major = *minor = 0;

    getGlVersion(&glMajor, &glMinor);
    if (glMajor == 1) {
        const char *extstr = (const char *)glGetString(GL_EXTENSIONS);
        if (extstr && strstr(extstr, "GL_ARB_shading_language_100")) {
            *major = 1;
            *minor = 0;
        }
    } else if (glMajor == 2) {
        const char *verstr = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
        if (!verstr || (sscanf(verstr, "%d.%d", major, minor) != 2)) {
            *major = *minor = 0;
            fprintf(stderr, "ERROR: invalid GL_SHADING_LANGUAGE_VERSION format!\n");
        }
    } else {
        fprintf(stderr, "ERROR: unexpected OpenGL version (%d.%d)!\n", glMajor, glMinor);
    }
}
