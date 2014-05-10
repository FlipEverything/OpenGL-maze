#ifndef GAMEDATA_H
#define GAMEDATA_H

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

#include <SOIL/SOIL.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <gameelement.h>

#define DEBUG false
using namespace std;

class GameData
{
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
    vector<GLdouble> camera;
    vector<GLdouble> center;
    vector<GLdouble> backupCamera;
    vector<GLdouble> backupCenter;
    int speed;
    bool isUpperView;
    bool isTextureActive;

    GameElement* wall;
    GameElement* floor;

    GLuint *textures;
    int numberOfTextures;
    int currentTexture;

    GameData();
    ~GameData();
    void readMaze();
    int loadTexture(const char *fileName);
    void genTextures(int numberOfTextures);
};

#endif // GAMEDATA_H
