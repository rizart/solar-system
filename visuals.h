#include <climits>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// -------- Model Structs

struct point {
    float x;
    float y;
    float z;
};

struct face {
    int vtx[3];
};

struct model {
    vector<point> obj_points;
    vector<face> obj_faces;
};

// -------- Callback Functions

void Render();
// The function responsible for drawing everything in the
// OpenGL context associated to a window.

void Resize(int, int);
// Handle the window size changes and define the world coordinate
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Keyboard(unsigned char, int, int);
// Function for handling keyboard events.

void Mouse(int, int, int, int);
// Function for handling mouse events.

void Idle();
