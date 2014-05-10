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

    camera = std::vector<GLdouble>(3, 0.0);
    center = std::vector<GLdouble>(3, 0.0);

    camera[0] = sizeX*3/2;
    camera[1] = -50;
    camera[2] = sizeX;

    center[0] = sizeX*3/2;
    center[1] = sizeX;
    center[2] = sizeX;

    wall = new GameElement();
    floor = new GameElement();
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

void GameData::genTextures(int numberOfTextures){
    this->numberOfTextures = numberOfTextures;
    this->currentTexture = 0;
    textures = new GLuint[numberOfTextures];

    // ask opengl to create textures
    glGenTextures(numberOfTextures, textures);
}

int GameData::loadTexture(const char *fileName)
{
    int twidth, theight;
    // load picture with soil
    unsigned char* image = SOIL_load_image(fileName, &twidth, &theight, 0, SOIL_LOAD_RGB);

    // bind it
    glBindTexture(GL_TEXTURE_2D, textures[currentTexture]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    delete image;

    return currentTexture++;
}
