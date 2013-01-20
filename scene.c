
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#include <wingdi.h>
#define GLUT_DISABLE_ATEXIT_HACK
#else
#include <sys/time.h>
#endif /* _WIN32 */

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

#include "util.h"
#include "scene.h"

static GLuint program;
static GLuint cameraPositionLocation;
static GLuint lightPositionLocation;
static GLuint lightColorLocation;
static GLuint cylinderBufferId;
static unsigned int cylinderNumVertices;

static float cameraPosition[3];

#define NUM_LIGHTS 3

static float lightPosition[NUM_LIGHTS * 3];
static float lightColor[NUM_LIGHTS * 3];
static float lightRotation;

static unsigned int getTicks(void)
{
#ifdef _WIN32
    return GetTickCount();
#else
    struct timeval t;

    gettimeofday(&t, NULL );

    return (t.tv_sec * 1000) + (t.tv_usec / 1000);
#endif /* _WIN32 */
}

static void createCylinder(unsigned int divisions)
{
    const int floatsPerVertex = 6;
    unsigned int i, size;
    float *v;

    cylinderNumVertices = (divisions + 1) * 2;
    size = floatsPerVertex * cylinderNumVertices;

    v = malloc(sizeof(float) * size);
    for (i = 0; i <= divisions; ++i) {
        float r = ((M_PI * 2.0f) / (float) divisions) * (float) i;
        unsigned int index1 = i * 2 * floatsPerVertex;
        unsigned int index2 = index1 + floatsPerVertex;

        v[index1 + 0] = cosf(r);
        v[index1 + 1] = 1.0f;
        v[index1 + 2] = -sinf(r);
        v[index2 + 0] = cosf(r);
        v[index2 + 1] = -1.0f;
        v[index2 + 2] = -sinf(r);

        v[index1 + 3] = cosf(r);
        v[index1 + 4] = 0.0f;
        v[index1 + 5] = -sinf(r);
        v[index2 + 3] = v[index1 + 3];
        v[index2 + 4] = v[index1 + 4];
        v[index2 + 5] = v[index1 + 5];
    }

    glGenBuffers(1, &cylinderBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, cylinderBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, v, GL_STATIC_DRAW);
    free(v);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(float) * floatsPerVertex, 0);
    glNormalPointer(GL_FLOAT, sizeof(float) * floatsPerVertex, (const GLvoid *) (sizeof(float) * 3));
}

void sceneCycle(void);

void sceneInit(void)
{
    GLint result;
    GLuint shader;

    program = glCreateProgram();

    shader = compileShader(GL_VERTEX_SHADER, "shaders/brick_shader.vp");
    if (shader != 0) {
        glAttachShader(program, shader);
        // Mark the shader for deletion when the program is destroyed.
        glDeleteShader(shader);
    }

    shader = compileShader(GL_FRAGMENT_SHADER, "shaders/brick_shader.fp");
    if (shader != 0) {
        glAttachShader(program, shader);
        // Mark the shader for deletion when the program is destroyed.
        glDeleteShader(shader);
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

    cameraPositionLocation = glGetUniformLocation(program, "cameraPosition");
    lightPositionLocation = glGetUniformLocation(program, "lightPosition");
    lightColorLocation = glGetUniformLocation(program, "lightColor");

    lightColor[0] = 1.0f;
    lightColor[1] = 0.0f;
    lightColor[2] = 0.0f;
    lightColor[3] = 0.0f;
    lightColor[4] = 1.0f;
    lightColor[5] = 0.0f;
    lightColor[6] = 0.0f;
    lightColor[7] = 0.0f;
    lightColor[8] = 1.0f;

    createCylinder(36);

    lightRotation = 0.0f;
    sceneCycle();

    cameraPosition[0] = 0.0f;
    cameraPosition[1] = 0.0f;
    cameraPosition[2] = 4.0f;

    glLoadIdentity();
    glTranslatef(-cameraPosition[0], -cameraPosition[1], -cameraPosition[2]);
}

void sceneRender(void)
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
    glUniform3fv(cameraPositionLocation, 1, cameraPosition);
    glUniform3fv(lightPositionLocation, NUM_LIGHTS, lightPosition);
    glUniform3fv(lightColorLocation, NUM_LIGHTS, lightColor);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, cylinderNumVertices);

    glUseProgram(0);

    for (i = 0; i < NUM_LIGHTS; ++i) {
        glPushMatrix();
        glTranslatef(lightPosition[i * 3 + 0], lightPosition[i * 3 + 1], lightPosition[i * 3 + 2]);
        glColor3fv(lightColor + (i * 3));
        glutSolidSphere(0.04, 36, 36);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void sceneCycle(void)
{
    static unsigned int prevTicks = 0;
    unsigned int ticks;
    float secondsElapsed;
    int i;

    if (prevTicks == 0)
        prevTicks = getTicks();
    ticks = getTicks();
    secondsElapsed = (float) (ticks - prevTicks) / 1000.0f;
    prevTicks = ticks;

    lightRotation += (M_PI / 4.0f) * secondsElapsed;
    for (i = 0; i < NUM_LIGHTS; ++i) {
        const float radius = 1.75f;
        float r = (((M_PI * 2.0f) / (float) NUM_LIGHTS) * (float) i) + lightRotation;

        lightPosition[i * 3 + 0] = cosf(r) * radius;
        lightPosition[i * 3 + 1] = cosf(r) * sinf(r);
        lightPosition[i * 3 + 2] = sinf(r) * radius;
    }

    glutPostRedisplay();
}
