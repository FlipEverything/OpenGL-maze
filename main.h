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
    #include <stdio.h>
    #include <malloc.h>
    #include <string.h>
    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <string>
    #include <vector>
    #include <SOIL/SOIL.h>
    #include <Loader.h>


    // Define a constant for the value of PI
    #define GL_PI 3.1415f
    #define DEBUG false

    class GameData {
    public:

        // object sizes
        const static float sizeX = 50.0f; // width
        const static float sizeY = 50.0f; // height
        const static float sizeZ = 50.0f; // length

        //window
        const static int windowHeight = 800;
        const static int windowWidth = 1000;

        // constants
        const static int cubeCorners = 8;
        const static int cubeSides = 6;
        const static int oneVertexCoordNum = 3;
        const static int oneTexCoordNum = 2;
        const static int triangleBySide = 2;
        const static int indexCount = 36;
        const static int textureCount = 8;
        const static int runningSpeed = 3;
        const static int walkingSpeed = 5;
        const static GLfloat upperViewHeight = 800.0;

        // maze
        int *mazeArray; // the maze (1 / 0) bit-matrix
        int mazeWidth; // dimension
        int mazeHeight; // dimension
        int mazeBlockCount; // number of blocks (1)

        //camera
        std::vector<GLdouble> camera;
        std::vector<GLdouble> center;
        std::vector<GLdouble> backupCamera;
        std::vector<GLdouble> backupCenter;
        int speed;
        bool isUpperView;
        bool isTextureActive;

        GameData()
        {
            mazeArray = NULL;
            mazeWidth = 0;
            mazeHeight = 0;
            mazeBlockCount = 0;
            isUpperView = true;
            speed = walkingSpeed;
            isTextureActive = true;

            camera = std::vector<GLdouble>(3, 0.0);
            center = std::vector<GLdouble>(3, 0.0);

            camera[0] = sizeX*3/2;
            camera[1] = -50;
            camera[2] = sizeX;

            center[0] = sizeX*3/2;
            center[1] = sizeX;
            center[2] = sizeX;

        }
        ~GameData()
        {
            free(mazeArray);
        }

        void readMaze();
    };

#endif // MAIN_H
