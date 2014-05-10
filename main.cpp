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
GLfloat lightPos[] = { 0.0f, 100.0f, 800.0f, 1.0f };

Loader loader[2];

/**
 * @brief loadTextures Load the textures
 */
void loadTextures() {
    int numberOfTextures = 2;
    int currentTexture = 0;
    maze.genTextures(numberOfTextures);

    currentTexture = maze.loadTexture("texture/stone.png");
    maze.wall->setTextureId(currentTexture);

    currentTexture = maze.loadTexture("texture/grass.png");
    maze.floor->setTextureId(currentTexture);
}

/**
 * @brief GenerateSide Generate vertex, index, texture coordinate for one side of the cube
 * @param coords Bit coordinates for the side generation (one side of the cube: 4 vertex)
 * @param i Current position in the maze
 * @param j Current position in the maze
 * @param count number of the current element
 */
void GenerateSide(int coords[], int i, int j, int count, int orientation, GameElement* element){
    int x, y, z, index, texture;

    // Vertex coordinates
    for (int k=0;k<4;k++) // 1 side - 4 vertex
    {
        x = j * maze.sizeX + (maze.sizeX * coords[k*3]);
        y = i * maze.sizeY + (maze.sizeY * coords[k*3+1]);
        z = coords[k*3+2] * maze.sizeZ;
        element->getVertices()->push_back( x ); // x coord
        element->getVertices()->push_back( y ); // y coord
        element->getVertices()->push_back( z ); // z coord
        if (DEBUG) { cout << "Vertex " <<  " x: " << x << " y: " << y << "  z: " << z << "\n"; }

        // Normalvector coordinates
        if (orientation==0){
            element->getNormals()->push_back(1.0);
            element->getNormals()->push_back(0.0);
            element->getNormals()->push_back(0.0);
        } else if (orientation==1){
            element->getNormals()->push_back(0.0);
            element->getNormals()->push_back(1.0);
            element->getNormals()->push_back(0.0);
        } else if (orientation==2){
            element->getNormals()->push_back(0.0);
            element->getNormals()->push_back(0.0);
            element->getNormals()->push_back(1.0);
        } else if (orientation==-1){
            element->getNormals()->push_back(-1.0);
            element->getNormals()->push_back(0.0);
            element->getNormals()->push_back(0.0);
        } else if (orientation==-2){
            element->getNormals()->push_back(0.0);
            element->getNormals()->push_back(0.0);
            element->getNormals()->push_back(-1.0);
        } else if (orientation==-3){
            element->getNormals()->push_back(0.0);
            element->getNormals()->push_back(-1.0);
            element->getNormals()->push_back(0.0);
        }

    }

    // Index coordinates
    int indicePattern[] = {0,1,2, 2,3,0}; // Order of the vertexes  (sequence of drawing)
    if (DEBUG) { cout << "Index: "; }
    for (int k=0;k<6;k++)
    {
        index = indicePattern[k] + count * 4;
        element->getIndices()->push_back( index );
        if (DEBUG) { cout << index << " "; }
    }
    if (DEBUG) { cout << "\n "; }


    // Texture coordinates
    int texturePattern[] = {0,0, 0,1, 1,1, 1,0}; // Pairing the side vertexes to the image corners

    if (DEBUG) { cout << "Texture: "; }
    for (int k=0;k<8;k++)
    {
        texture = texturePattern[k];
        element->getTexCoords()->push_back( texture );
        if (DEBUG) { cout << texture << " "; }
    }
    if (DEBUG) { cout << "\n "; }

}

/**
 * @brief GenerateVertices Read the maze array then draw it.
 */
void GenerateCubes()
{

    // Draw the maze
      int count = 0;
      for (int i=0; i<maze.mazeHeight; i++)
      {
          for (int j=0; j<maze.mazeWidth; j++)
          {
              if (DEBUG) { cout << "Maze input: " << "(i: " << i << ") (j: " << j << ") = " << maze.mazeArray[i*maze.mazeWidth+j] << "\n"; }
              // Render every cube of the maze
              if (maze.mazeArray[i*maze.mazeWidth+j]==1)
              {
                  // Sides of the cube
                  int front[] = {0,1,0, 0,1,1, 1,1,1, 1,1,0};
                  int top[] = {0,1,1, 0,0,1, 1,0,1, 1,1,1};
                  int left[] = {0,0,0, 0,0,1, 0,1,1, 0,1,0};
                  int right[] = {1,1,0, 1,1,1, 1,0,1, 1,0,0};
                  int back[] = {1,0,0, 1,0,1, 0,0,1, 0,0,0};

                  // Call the vertex generator
                  GenerateSide(front, i, j, count++, -1, maze.wall);
                  GenerateSide(top, i, j, count++, 2, maze.wall);
                  GenerateSide(left, i, j, count++, -1, maze.wall);
                  GenerateSide(right, i, j, count++, -3, maze.wall);
                  GenerateSide(back, i, j, count++, 2, maze.wall);
              }

          }
      }

      int floorCount = 0;
      // Draw the floor
      // width x height pieces
      if (DEBUG) { cout << "\n\nFloor:\n"; }
      for (int i=0; i<maze.mazeHeight; i++)
      {
          for (int j=0; j<maze.mazeWidth; j++)
          {
              // Draw a panel for the every block of the maze
              int floor[] = {0,1,0, 0,0,0, 1,0,0, 1,1,0};
              GenerateSide(floor, i, j, floorCount++, 2, maze.floor);
          }
      }
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

    gluLookAt(  maze.camera[0], maze.camera[1], maze.camera[2], // camera
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
    maze.wall->render(maze.textures); // wall
    maze.floor->render(maze.textures); // floor

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


    GenerateCubes();
    glewInit();

    glClearColor( 0.878f, 0.878f, 0.878f, 1.0f ); //background

    loadTextures();

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

    GLfloat dirX = maze.camera[0] - maze.center[0];
    GLfloat dirZ = maze.camera[1] - maze.center[1];
    // movement speed
    GLdouble length = sqrt(dirX * dirX + dirZ * dirZ) / maze.speed;

    // arrow keys
    if(key == GLUT_KEY_UP) { // move
        maze.center[0] -= dirX / length;
        maze.center[1] -= dirZ / length;
        maze.camera[0] -= dirX / length;
        maze.camera[1] -= dirZ / length;
    }

    if(key == GLUT_KEY_DOWN) { // move
        maze.center[0] += dirX / length;
        maze.center[1] += dirZ / length;
        maze.camera[0] += dirX / length;
        maze.camera[1] += dirZ / length;
    }

    if(key == GLUT_KEY_LEFT) { // rotare
        maze.center[0] += dirZ / (length / 2);
        maze.center[1] -= dirX / (length / 2);
    }

    if(key == GLUT_KEY_RIGHT) { // rotate
        maze.center[0] -= dirZ / (length / 2);
        maze.center[1] += dirX / (length / 2);
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
  // ...
  int state;

  printf("Billentyu lenyomva, kodja %c, pozicio (%d,%d). ", key, x, y);
  state = glutGetModifiers();

  if(state & GLUT_ACTIVE_SHIFT){
    printf("SHIFT lenyomva. ");
    maze.speed = maze.runningSpeed;
  } else {
    maze.speed = maze.walkingSpeed;
  }

  if(state & GLUT_ACTIVE_CTRL)
    printf("CTRL lenyomva. ");
  if(state & GLUT_ACTIVE_ALT)
    printf("ALT lenyomva. ");
  printf("\n");
  fflush(stdout);



  glutPostRedisplay();
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
    glutTimerFunc(100, Timer, 1);
    glutDisplayFunc(RenderScene);
    SetupRC();
    glutMainLoop();

    return 0;
}
