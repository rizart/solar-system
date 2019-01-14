CC = g++
CCFLAGS = -lGL -lGLU -lglut -Wall

all:
	$(CC) main.cpp visuals.cpp $(CCFLAGS) -o solar
clean:
	rm -rf solar
