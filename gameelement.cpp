#include "gameelement.h"


void GameElement::printVector(vector<GLfloat> value, int numberOfCoords)
{
    int counter = 1;
    for( vector<GLfloat>::const_iterator i = value.begin(); i != value.end(); ++i)
    {
        cout << *i << ' ';
        if (counter++%numberOfCoords==0)
        {
            cout << endl;
        }
    }
}

void GameElement::printVector(vector<GLuint> value, int numberOfCoords)
{
    int counter = 1;
    for( vector<GLuint>::const_iterator i = value.begin(); i != value.end(); ++i)
    {
        cout << *i << ' ';
        if (counter++%numberOfCoords==0)
        {
            cout << endl;
        }

    }
}

void GameElement::move(GLfloat x, GLfloat y, GLfloat z)
{
    /*for( vector<GLfloat>::const_iterator i = vertices->begin(); i != vertices->end(); ++i)
    {
        cout << *i << ' ';
        vertices->push_back(*i + x);

    }*/

    for (int i = 0; i < vertices.size(); i++)
    {
        //(&vertices)[i] += 1;
        cout << vertices[i];
    }
}

GameElement::GameElement() {}
GameElement::~GameElement() { }

void GameElement::render(GLuint textures[])
{
    // render the vertex array
    glBindTexture(GL_TEXTURE_2D, textures[textureId]); // texture data
    glVertexPointer(3, GL_FLOAT, 0, vertices.data()); // vertex
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data()); // texture coordinate
    glNormalPointer(GL_FLOAT, 0, normals.data()); // normalvector
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data()); // index
}


/**
 * @brief GenerateSide Generate vertex, index, texture coordinate for one side of the cube
 * @param coords Bit coordinates for the side generation (one side of the cube: 4 vertex)
 * @param i Current position in the maze
 * @param j Current position in the maze
 * @param count number of the current element
 */
void GameElement::GenerateSide(int coords[], int i, int j, int count, int orientation, float sizeX, float sizeY, float sizeZ){
    int x, y, z, index, texture;

    // Vertex coordinates
    for (int k=0;k<4;k++) // 1 side - 4 vertex
    {
        x = j * sizeX + (sizeX * coords[k*3]);
        y = i * sizeY + (sizeY * coords[k*3+1]);
        z = coords[k*3+2] * sizeZ;
        vertices.push_back( x ); // x coord
        vertices.push_back( y ); // y coord
        vertices.push_back( z ); // z coord

        // Normalvector coordinates
        if (orientation==0){
            normals.push_back(1.0);
            normals.push_back(0.0);
            normals.push_back(0.0);
        } else if (orientation==1){
            normals.push_back(0.0);
            normals.push_back(1.0);
            normals.push_back(0.0);
        } else if (orientation==2){
            normals.push_back(0.0);
            normals.push_back(0.0);
            normals.push_back(1.0);
        } else if (orientation==-1){
            normals.push_back(-1.0);
            normals.push_back(0.0);
            normals.push_back(0.0);
        } else if (orientation==-2){
            normals.push_back(0.0);
            normals.push_back(0.0);
            normals.push_back(-1.0);
        } else if (orientation==-3){
            normals.push_back(0.0);
            normals.push_back(-1.0);
            normals.push_back(0.0);
        }

    }

    // Index coordinates
    int indicePattern[] = {0,1,2, 2,3,0}; // Order of the vertexes  (sequence of drawing)
    for (int k=0;k<6;k++)
    {
        index = indicePattern[k] + count * 4;
        indices.push_back( index );

    }
    // Texture coordinates
    int texturePattern[] = {0,0, 0,1, 1,1, 1,0}; // Pairing the side vertexes to the image corners

    for (int k=0;k<8;k++)
    {
        texture = texturePattern[k];
        texCoords.push_back( texture );
    }

}


bool GameElement::load(const char *filename){
    int val;
    vector< vec3 > temp_vertices;
    vector< vec2 > temp_uvs;
    vector< vec3 > temp_normals;

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

    FILE* file = fopen(filename, "r");
    if (file == NULL){
        printf("Impossible to open the file !\n");
    } else {

        while ( 1 ){
            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.

            // else : parse lineHeader
            if ( strcmp( lineHeader, "v" ) == 0 ){
                vec3 vertex;
                val = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                temp_vertices.push_back(vertex);
                vertices.push_back(vertex.x);
                vertices.push_back(vertex.y);
                vertices.push_back(vertex.z);
            }else if ( strcmp( lineHeader, "vt" ) == 0 ){
                vec2 uv;
                val = fscanf(file, "%f %f\n", &uv.x, &uv.y );
                temp_uvs.push_back(uv);
            }else if ( strcmp( lineHeader, "vn" ) == 0 ){
                vec3 normal;
                val = fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
                temp_normals.push_back(normal);
            }else if ( strcmp( lineHeader, "f" ) == 0 ){
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                if (matches != 9){
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                } else {
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices    .push_back(uvIndex[0]);
                    uvIndices    .push_back(uvIndex[1]);
                    uvIndices    .push_back(uvIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                }

            }else{
                // Probably a comment, eat up the rest of the line
                char stupidBuffer[1000];
                char* ignore = fgets(stupidBuffer, 1000, file);
            }

        }

        // For each vertex of each triangle
        for( unsigned int i=0; i<vertexIndices.size(); i++ ){
            // Get the indices of its attributes
            //unsigned int vertexIndex = vertexIndices[i];
            unsigned int uvIndex = uvIndices[i];
            unsigned int normalIndex = normalIndices[i];

            // Get the attributes thanks to the index
            //vec3 vertex = temp_vertices[ vertexIndex-1 ];
            vec2 uv = temp_uvs[ uvIndex-1 ];
            vec3 normal = temp_normals[ normalIndex-1 ];

            // Put the attributes in buffers
            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);

            texCoords.push_back(uv.x);
            texCoords.push_back(uv.y);


            indices.push_back(vertexIndices[i]-1);

        }


    }

    return true;
}


vector<GLfloat> GameElement::getNormals() const
{
    return normals;
}

void GameElement::setNormals(const vector<GLfloat> &value)
{
    normals = value;
}

vector<GLfloat> GameElement::getTexCoords() const
{
    return texCoords;
}

void GameElement::setTexCoords(const vector<GLfloat> &value)
{
    texCoords = value;
}

vector<GLuint> GameElement::getIndices() const
{
    return indices;
}

void GameElement::setIndices(const vector<GLuint> &value)
{
    indices = value;
}

vector<GLfloat> GameElement::getVertices() const
{
    return vertices;
}

void GameElement::setVertices(const vector<GLfloat> &value)
{
    vertices = value;
}

int GameElement::getTextureId() const
{
    return textureId;
}

void GameElement::setTextureId(int value)
{
    textureId = value;
}
