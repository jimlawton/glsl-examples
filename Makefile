CC = gcc
CFLAGS = -Wall -Werror -DGL_GLEXT_PROTOTYPES
LDFLAGS = -pthread -lm -lGL -lglut

all: glsl_compile glsl_example

glsl_example: glsl_example.o scene.o util.o
	$(CC) $(LDFLAGS) $^ -o $@

glsl_compile: glsl_compile.o util.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f glsl_example *.o

