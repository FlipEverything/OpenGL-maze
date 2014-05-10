#ifndef MAIN_H
#define MAIN_H

    #  ifdef WIN32
    #    include <windows.h>
    #    include <wingdi.h>
    #  endif /* WIN32 */
    #  ifdef __APPLE__
    #    include <GLUT/glew.h>
    #    include <GLUT/glut.h>
    #  else
    #    include <GL/glew.h>
    #    include <GL/glut.h>
    #  endif


    #include <math.h>
    #include <malloc.h>
    #include <string>
    #include <vector>

    // Define a constant for the value of PI
    #define GL_PI 3.1415f
    #define DEBUG false

    using namespace std;

    #include <gamedata.h>
    #include <element.h>

void keyUp();
void keyOperations();
#endif // MAIN_H
