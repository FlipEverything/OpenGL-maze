/*
 * Dobó László
 * DOLSAAT.SZE
 *
 * Kötelező program
 * Fejlett Grafikai algoritmusok
 * 2014
 *
 * Labirintus generálás: vertextömb
 *
 * Fejlesztőkörnyezet: Ubuntu 13.10, Qt Creator (Fordítás Qt-val)
 * Függvénykönyvtárak: GLEW GL glut GLU SOIL
 *
 * A labirintus szerkezetének generálása külön történik, github-on elérhető c alkalmazás segítségével.
 * Forrás: https://github.com/ccouzens/maze
 * Az alkalmazást úgy módosítottam, hogy a konzol helyett fájlba írja a kimenetet a megadott formátumban.
 * Első sor: width height\n
 * Ezután width * height mátrix, mely 0 (folyosó) és 1 (fal) elemekből áll, ez reprezentálja a labirintust.
 *
 * Kijárat és bejárat szerepel a labirintusban, mindig el lehet érni a célt.
 *
 * A labirintust a maze.txt fájlba várja a program, majd innen beolvassa és memóriában eltárolja.
 *
 * A labirintus rajzolása vertextömb segítségével történik, így ezeket legenerálom: Vertextömb, indextömb, textúratömb.
 * Textúra alkalmazva az objektumokon: Soartex Fanver resource pack for Minecraft
 * Forrás: http://www.minecraftforum.net/topic/824150-17x-soartex-fanver-smooth-and-clean-released/
 *
 * Világítás alkalmazva.
 * Irányítás: nyilakkal, fps-like. (kamera és irány mozgatás)
 *  Jobb, bal: forgás
 *  Előre, hátra: mozgás
 * "F1" gomb lenyomásával felülnézeti kameraállás
 *
 * DEBUG konzolra ír
 *
 * További források: openGL wiki, stackoverflow, számítógépes grafika kötelező program
 *
 */


#include <main.h>

GameData maze;

GLfloat glTime;

GLfloat mouseX = -1.0, mouseY = -1.0;
GLfloat centerX = 0.0;
GLfloat centerY = 0.0;

// lights
GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPos[] = { 0.0f, -5.0f, 10.0f, 1.0f };

bool* keyStates = new bool[256]();
bool running;

/**
 * @brief loadTextures Load the textures
 */
void loadTextures() {
    int numberOfTextures = 3;
    int currentTexture = 0;
    maze.genTextures(numberOfTextures);

    maze.loadTexture("texture/stone.png", currentTexture);
    maze.wall.setTextureId(currentTexture++);

    maze.loadTexture("texture/grass.png", currentTexture);
    maze.floor.setTextureId(currentTexture++);

    maze.loadTexture("texture/char.png", currentTexture);
    maze.player.setTextureId(currentTexture++);
}

/**
 * @brief ChangeSizePerspective Move the camera
 * @param w window dimension
 * @param h window dimension
 */
void ChangeSizePerspective(GLsizei w, GLsizei h) {
    GLfloat fAspect;

    if(h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w/(GLfloat)h;

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Produce the perspective projection
    gluPerspective( 90.0f,    // fovy
                    fAspect,  // aspect
                    0.1f,     // zNear
                    1000.0     // zFar
                    );

    gluLookAt(  maze.camera[0], maze.camera[1], maze.camera[2], // eye / camera
                maze.center[0], maze.center[1], maze.center[2], // center
                0.0, 0.0, 1.0                                   // up
                );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


/**
 * @brief RenderScene Called to draw scene
 */
void RenderScene(void)
{
    keyOperations();
    if (!maze.isTextureActive) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); } else { glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); }

    // Move the camera every scene
    ChangeSizePerspective(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);


    // draw the elements
    maze.wall.render(maze.textures); // wall
    maze.floor.render(maze.textures); // floor
    maze.player.render(maze.textures); // player

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    // cleanup the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Display the results
    glutSwapBuffers();
}

/**
 * @brief SetupRC This function does any needed initialization on the rendering context.
 */
void SetupRC()
{

    //lights
    glEnable(GL_LIGHTING);
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambientLight );
    // Setup and enable light 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);

    // All materials hereafter have full specular reflectivity
    // with a high shine
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    glMateriali(GL_FRONT,GL_SHININESS, 20);


    glEnable(GL_DEPTH_TEST);	// Hidden surface removal
    glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
    glShadeModel(GL_SMOOTH);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);


    loadTextures();
    maze.generateCubes();
    maze.wall.calculate();

    maze.player.load("model/MinecraftPlayer.obj");
    //maze.player.printVector(maze.player.getTexCoords(),2);

    maze.player.move(maze.camera[0], 0.4f, 0, maze.wall.getBoundaryBox(), true);

    glewInit();
    glClearColor( 0.878f, 0.878f, 0.878f, 1.0f ); //background

    glEnable(GL_TEXTURE_2D);
}

/**
 * @brief SpecialKeys Move the camera
 * @param key
 * @param x
 * @param y
 */
void SpecialKeys(int key, int x, int y)
{

    GLfloat dirX = 0;
    GLfloat dirZ = 0;

    // arrow keys
    /*if(key == GLUT_KEY_UP) { // move
        dirZ = maze.sizeZ/maze.speed;
    }

    if(key == GLUT_KEY_DOWN) { // move
        dirZ = -maze.sizeZ/maze.speed;
    }*/

    if(key == GLUT_KEY_LEFT) { // move
        dirX = -maze.sizeX/20;

        maze.center[1] -= sin(dirX * M_PI / 180) * cos(dirX * M_PI / 180);
        maze.center[0] -= -cos(dirX * M_PI / 180) * cos(dirX * M_PI / 180);
    }

    if(key == GLUT_KEY_RIGHT) { // move
        dirX = maze.sizeX/20;

        maze.center[1] += sin(dirX * M_PI / 180) * cos(dirX * M_PI / 180);
        maze.center[0] += -cos(dirX * M_PI / 180) * cos(dirX * M_PI / 180);
    }    




    // F1: switch from fps to upper camera (and backwards)
    if(key == GLUT_KEY_F1) {
        if(maze.isUpperView) {
            maze.backupCamera = maze.camera; // save the current camera
            maze.backupCenter = maze.center;

            maze.camera[2] = maze.upperViewHeight; // set the upper angle

        } else {
            maze.camera[2] = maze.backupCamera[2]; // load the backup camera angle

        }
        maze.isUpperView = !maze.isUpperView;
    }
    if(key == GLUT_KEY_F2) {
        maze.isTextureActive = !maze.isTextureActive;
    }

    glutPostRedisplay();
}

/**
 * @brief ChangeSize Orthographic projection
 * @param w width
 * @param h height
 */
void ChangeSize(int w, int h)
{
    GLfloat nRange = 100.0f;
    // Prevent a divide by zero
    if(h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0,0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h)
        glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else
        glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y)
{
  keyStates[key] = true;
}

void keyUp (unsigned char key, int x, int y)
{
    keyStates[key] = false;
}

void keyOperations(){
    GLfloat dirX = 0;
    GLfloat dirZ = 0;

    int speed = maze.speed;

    // arrow keys
    if(keyStates['w']) { // move
        dirZ = maze.sizeZ/speed;
    }

    if(keyStates['s']) { // move
        dirZ = -maze.sizeZ/speed;
    }

    if(keyStates['a']) { // move
        dirX = -maze.sizeX/speed;
    }

    if(keyStates['d']) { // move
        dirX = maze.sizeX/speed;
    }

    if (dirZ!=0 || dirX != 0)
    {
        bool moved = maze.player.move(dirX, dirZ, 0, maze.wall.getBoundaryBox(), false);
        if (moved)
        {
            maze.camera[0] += dirX;
            maze.camera[1] += dirZ;

            maze.center[0] += dirX;
            maze.center[1] += dirZ;
        }
    }




}

void Timer(int value)
{
  glTime += 1;
  glutPostRedisplay();
  glutTimerFunc(100, Timer, value);
}


int main(int argc, char* argv[])
{

    // load the maze
    maze.readMaze();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(maze.windowWidth, maze.windowHeight);
    glutCreateWindow("Dobo Laszlo DOLSAAT.SZE | Fejlett Grafika 2014");
    glutReshapeFunc(ChangeSizePerspective); // Perspektiv vetites
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(keyUp);
    glutTimerFunc(100, Timer, 1);
    glutDisplayFunc(RenderScene);
    SetupRC();
    glutMainLoop();

    return 0;
}
