#include "visuals.h"
#include <GL/glut.h>

int main(int argc, char *argv[]) {
    // initialize GLUT library state
    glutInit(&argc, argv);

    // window manager decides where the window will initially appear
    glutInitWindowPosition(-1, -1);

    // define the main window size  - WVGA (768x480)
    glutInitWindowSize(768, 480);

    // GLUT_RGBA - Bit mask to select an RGBA mode window
    // GLUT_DOUBLE - Bit mask to select a double buffered window
    // GLUT_DEPTH - Bit mask to select a window with a depth buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    // create and label the main window
    glutCreateWindow("solar system");

    // configure various properties of the OpenGL rendering context
    Setup();

    // Callbacks for the GL and GLUT events:

    // -- the display callback function
    // -- usage: void glutDisplayFunc(void (*func)(void));
    glutDisplayFunc(Render);

    // -- the reshape callback function
    // -- usage: void glutReshapeFunc(void (*func)(int width, int height));
    glutReshapeFunc(Resize);

    // -- the idle callback function
    // -- usage: void glutIdleFunc(void (*func)(void))
    glutIdleFunc(Idle);

    // -- the keyboard callback function
    // -- usage:
    //      void glutKeyboardFunc(void (*func)(unsigned char key,int x, int y));
    glutKeyboardFunc(Keyboard);

    // -- the mouse callback function
    // -- usage:
    //      void glutMouseFunc(void (*func)(int button, int state,int x, int
    //      y));
    glutMouseFunc(Mouse);

    // enter main event handling loop
    glutMainLoop();

    return 0;
}
