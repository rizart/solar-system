#include "visuals.h"
#include <GL/glut.h>

// definitions
#define SUN_RADIUS 4.0
#define STAR_COUNT 700
#define STARS_RADIUS 0.05

// model
model md;

// animation
static bool animate = true;

// camera movement
static float tx = 0.0;
static float ty = 0.0;
static float tz = 0.0;

// planets
static float planet_rotx = 0.0;
static float rotating_speed = 1.5;

// sun
static float sun_radius_now = SUN_RADIUS;
static float sun_flickering_speed = 0.08;

// stars
static float stars_radius_now = STARS_RADIUS;
static float stars_flickering_speed = 0.01;
point *stars[STAR_COUNT];

void Resize(int w, int h) {
    // define the visible area of the window ( in pixels )
    if (h == 0)
        h = 1;
    glViewport(0, 0, w, h);

    // setup viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (float)w / (float)h, 1.0, 500.0);
}

void Mouse(int button, int state, int x, int y) {
    // wheel reports as button 3(scroll up) and button 4(scroll down)
    if ((button == 3) || (button == 4)) {
        // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
        if (state == GLUT_UP)
            return; // Disregard redundant GLUT_UP events
        // If a GLUT_DOWN callback for a specific button is triggered,
        // the program can assume a GLUT_UP callback
        // for the same button will be generated
        button == 3 ? tz += 0.5 : tz -= 0.5;
    }
    glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == 'a')
        ty += 0.5; // move left
    if (key == 'd')
        ty -= 0.5; // move right
    if (key == 'w')
        tx += 0.5; // move up
    if (key == 's')
        tx -= 0.5; // move down
    if (key == '=')
        tz += 0.5; // zoom in
    if (key == '-')
        tz -= 0.5; // zoom out
    if (key == 'q')
        exit(EXIT_SUCCESS); // quit program
    if (key == '\'')
        rotating_speed += 0.2;
    if (key == ';')
        rotating_speed -= 0.2;
    if (key == 'p') { // stop or start animation
        if (animate)
            animate = false;
        else
            animate = true;
    }
    glutPostRedisplay();
}

void Idle() {
    if (animate) {
        // sun flickering
        if (sun_radius_now < SUN_RADIUS)
            sun_flickering_speed = -sun_flickering_speed;
        if (sun_radius_now > 5.0)
            sun_flickering_speed = -sun_flickering_speed;
        sun_radius_now += sun_flickering_speed;

        // stars flickering
        if (stars_radius_now < STARS_RADIUS)
            stars_flickering_speed = -stars_flickering_speed;
        if (stars_radius_now > 0.1)
            stars_flickering_speed = -stars_flickering_speed;
        stars_radius_now += stars_flickering_speed;

        // planets rotating
        planet_rotx += rotating_speed;
    }
    glutPostRedisplay();
}

void render_sun() {
    glPushMatrix();
    glColor3f(1.0, 0.90, 0.48);
    glutSolidSphere(SUN_RADIUS, 30, 24);
    glColor4f(0.83, 0.74, 0.05, 0.5);
    glutSolidSphere(sun_radius_now, 30, 24);
    glPopMatrix();
}

void render_stars() {
    srand(time(NULL));
    for (int i = 0; i != STAR_COUNT; i++) {
        glPushMatrix();
        glTranslatef(stars[i]->x, stars[i]->y, stars[i]->z);
        glColor3f(1, 1, 1);
        glutSolidSphere(STARS_RADIUS, 30, 24);
        glColor4f(1, 1, 1, 0.5);
        glutSolidSphere(stars_radius_now, 30, 24);
        glPopMatrix();
    }
}

void display_planet(float size) {
    glPushMatrix();
    glScalef(size, size, size);
    glBegin(GL_TRIANGLES);
    for (unsigned int f = 0; f != md.obj_faces.size(); f++) {
        for (int i = 0; i != 3; i++) {
            glVertex3f(md.obj_points[md.obj_faces[f].vtx[i] - 1].x,
                       md.obj_points[md.obj_faces[f].vtx[i] - 1].y,
                       md.obj_points[md.obj_faces[f].vtx[i] - 1].z);
        }
    }
    glEnd();
    glPopMatrix();
}

void render_planets() {
    float slfr = 2;

    // cyan
    glPushMatrix();
    glColor3f(0.0, 1.0, 1.0);
    glRotatef(planet_rotx, 0.0, 1.0, 0.0);
    glTranslatef(14.0, 0.0, 0.0);
    glRotatef(slfr * planet_rotx, 1.0, 0.0, 0.0);
    display_planet(0.0010);
    glPopMatrix();

    // dark orange
    glPushMatrix();
    glColor3f(0.75, 0.26, 0.05);
    glRotatef(planet_rotx, 0.0, 1.0, 0.0);
    glTranslatef(-16.0, 0.0, 0.0);
    glRotatef(slfr * planet_rotx, 0.0, 1.0, 0.0);
    display_planet(0.0030);
    glPopMatrix();

    // dark grey
    glPushMatrix();
    glColor3f(0.66, 0.66, 0.66);
    glRotatef(planet_rotx, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 11.0, 0.0);
    glRotatef(slfr * planet_rotx, 0.0, 0.0, 1.0);
    display_planet(0.0015);
    glPopMatrix();

    // purple
    glPushMatrix();
    glColor3f(0.58, 0.0, 0.82);
    glRotatef(planet_rotx, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -12.0, 0.0);
    glRotatef(slfr * planet_rotx, 0.0, 1.0, 0.0);
    display_planet(0.0025);
    glPopMatrix();
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -35.0);

    GLfloat light_position[] = {0.0, 10.0, tz, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // camera position
    glTranslatef(0.0, 0.0, tz);
    glRotatef(tx, 1.0, 0.0, 0.0);
    glRotatef(ty, 0.0, 1.0, 0.0);

    // scene renderings
    render_stars();
    render_sun();
    render_planets();

    glutSwapBuffers();
}

void generateRandomStars() {
    for (int i = 0; i < STAR_COUNT; i++) {
        stars[i] = (point *)malloc(sizeof(point));
        float LO = -70.0;
        float HI = 70.0;
        stars[i]->x = LO + static_cast<float>(rand()) /
                               (static_cast<float>(RAND_MAX / (HI - LO)));
        stars[i]->y = LO + static_cast<float>(rand()) /
                               (static_cast<float>(RAND_MAX / (HI - LO)));
        stars[i]->z = LO + static_cast<float>(rand()) /
                               (static_cast<float>(RAND_MAX / (HI - LO)));
    }
}

void ReadFile(model *md) {
    ifstream obj_file("planet.obj");

    if (obj_file.fail())
        exit(1);

    string line;
    while (!obj_file.eof()) {
        obj_file >> line;
        if (!obj_file.fail()) {
            if (line == "v") {
                point p;
                obj_file >> p.x >> p.y >> p.z;
                md->obj_points.push_back(p);
            } else if (line == "f") {
                face f;
                int ignoreint;
                for (int i = 0; i != 3; i++) {
                    obj_file >> f.vtx[i];
                    obj_file.ignore(2);
                    obj_file >> ignoreint;
                }
                md->obj_faces.push_back(f);
            } else
                obj_file.ignore(INT_MAX, '\n');
        }
    }
    obj_file.close();
}

void Setup() {
    // readl .obj file
    ReadFile(&md);

    // generate random star positions
    generateRandomStars();

    // Parameter handling
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1);

    // polygon rendering mode
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Black background
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Set up light source
    GLfloat light_position[] = {0.0, 10.0, -35.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat ambientLight[] = {0.55, 0.55, 0.55, 1.0};
    GLfloat diffuseLight[] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}
