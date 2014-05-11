#include "gamedata.h"

GameData::GameData()
{
    mazeArray = NULL;
    mazeWidth = 0;
    mazeHeight = 0;
    mazeBlockCount = 0;
    isUpperView = true;
    speed = walkingSpeed;
    isTextureActive = true;

    camera = std::vector<GLfloat>(3, 0.0);
    center = std::vector<GLfloat>(3, 0.0);

    camera[0] = sizeX*3/2; //x +
    camera[1] = -sizeZ*2/3; //z -
    camera[2] = sizeY*6/5; //y +

    center[0] = camera[0]; //x
    center[1] = -camera[1]; //z
    center[2] = camera[2]-sizeY/2; //y

    numberOfTextures = 0;
}

GameData::~GameData()
{
    free(mazeArray);
}

/**
 * @brief GameData::readMaze Read the maze (bit matrix) from a text file.
 */
void GameData::readMaze()
{
    string line;
    string temp;
    ifstream ifs("data/maze.txt");
    getline(ifs, line); // first line: width height
    if (line.empty()){
        cout << "No file (data/maze.txt) or wrong format!" << endl;
    }
    istringstream iss(line);

    iss >> temp;
    mazeWidth = atoi(temp.c_str());
    iss >> temp;
    mazeHeight = atoi(temp.c_str());

    this->mazeArray = (int *)malloc(sizeof(int) * mazeWidth * mazeHeight); // allocating memory space for the mazeArray
    char *lineChar;

    int j = 0;
    while (getline(ifs, line)) // read the file line by line
    {
        lineChar = (char*)line.c_str(); // string to character array (one number - one element of the maze)

        for (int i=0; i<mazeWidth; i++)
        {
            int data = lineChar[i] - '0'; // convert to number
            this->mazeArray[j*mazeWidth+i] = data;
            if (data==1)
            {
                mazeBlockCount++; // count the number of blocks (wall)
            }
            if (DEBUG) { cout << this->mazeArray[j*mazeWidth+i]; }
        }
        if (DEBUG) { cout << "\n"; }
        j++;
    }
}

/**
 * @brief GenerateVertices Read the maze array then draw it.
 */
void GameData::generateCubes()
{

    // Draw the maze
      int count = 0;
      for (int i=0; i<mazeHeight; i++)
      {
          for (int j=0; j<mazeWidth; j++)
          {
              // Render every cube of the maze
              if (mazeArray[i*mazeWidth+j]==1)
              {
                  // Sides of the cube
                  int front[] = {0,1,0, 0,1,1, 1,1,1, 1,1,0};
                  int top[] = {0,1,1, 0,0,1, 1,0,1, 1,1,1};
                  int left[] = {0,0,0, 0,0,1, 0,1,1, 0,1,0};
                  int right[] = {1,1,0, 1,1,1, 1,0,1, 1,0,0};
                  int back[] = {1,0,0, 1,0,1, 0,0,1, 0,0,0};

                  // Call the vertex generator

                  wall.GenerateSide(front, i, j, count++, -1, sizeX, sizeY, sizeZ,false);
                  wall.GenerateSide(top, i, j, count++, 2, sizeX, sizeY, sizeZ,true);
                  wall.GenerateSide(left, i, j, count++, -1, sizeX, sizeY, sizeZ,false);
                  wall.GenerateSide(right, i, j, count++, -3, sizeX, sizeY, sizeZ,false);
                  wall.GenerateSide(back, i, j, count++, 2, sizeX, sizeY, sizeZ,false);
              }
              else if  (mazeArray[i*mazeWidth+j]==2) // support for the balls
              {
                  GLfloat x = j * sizeX + (sizeX/2);
                  GLfloat y = i * sizeY + (sizeY/2);
                  vec2 coord;
                  coord.x = x;
                  coord.y = y;
                  ballPositions.push_back(coord);
              }

          }
      }

      int floorCount = 0;
      // Draw the floor
      // width x height pieces
      for (int i=0; i<mazeHeight; i++)
      {
          for (int j=0; j<mazeWidth; j++)
          {
              // Draw a panel for the every block of the maze
              int floor_side[] = {0,1,0, 0,0,0, 1,0,0, 1,1,0};
              floor.GenerateSide(floor_side, i, j, floorCount++, 2, sizeX, sizeY, sizeZ,false);
          }
      }

}

/**
 * @brief GameData::genTextures Ask opengl to create textures
 * @param numberOfTextures
 */
void GameData::genTextures(int numberOfTextures){
    glGenTextures(numberOfTextures, textures);
}

/**
 * @brief GameData::loadTexture Load a texture from file to GL buffer
 * @param fileName the fileName
 * @param textureId the requested ID
 */
void GameData::loadTexture(const char *fileName, int textureId)
{
    int twidth, theight;
    // load picture with soil
    unsigned char* image = SOIL_load_image(fileName, &twidth, &theight, 0, SOIL_LOAD_RGB);

    // bind it
    glBindTexture(GL_TEXTURE_2D, textures[textureId]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    delete image;
}
