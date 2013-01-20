CC=gcc
CFLAGS=-Wall -Werror -DGL_GLEXT_PROTOTYPES
LDFLAGS=-pthread -lm -lGL -lglut
OBJS=glsl_example.o scene.o shader.o util.o

all: glsl_lighting

glsl_lighting:	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o glsl_example

clean:
	rm -f glsl_example
	rm -f $(OBJS)

