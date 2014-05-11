#include "element.h"


Element::Element() {}
Element::~Element() { }

/**
 * @brief Element::printVector Printing the input vector
 * @param value the vector
 * @param numberOfCoords
 */
void Element::printVector(vector<GLfloat> value, int numberOfCoords)
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

/**
 * @brief Element::printVector Printing the input vector
 * @param value the vector
 * @param numberOfCoords
 */
void Element::printVector(vector<GLuint> value, int numberOfCoords)
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

/**
 * @brief Element::render Binding texture and rendering the elements of the array
 * @param textures texture array with the IDs
 */
void Element::render(GLuint textures[])
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
void Element::GenerateSide(int coords[], int i, int j, int count, int orientation, float sizeX, float sizeY, float sizeZ, bool isBoundary){
    int x, y, z, index, texture;
    boundary box;

    // Vertex coordinates
    for (int k=0;k<4;k++) // 1 side - 4 vertex
    {
        x = j * sizeX + (sizeX * coords[k*3]);
        y = i * sizeY + (sizeY * coords[k*3+1]);
        z = coords[k*3+2] * sizeZ;
        vertices.push_back( x ); // x coord
        vertices.push_back( y ); // y coord
        vertices.push_back( z ); // z coord

        if (isBoundary){
            vec2 b;
            b.x = x;
            b.y = y;
            //cout << "x: " << x << "y: " << y << endl;
            box.point.push_back(b); // saving the coordinates for the boundary rectangle
        }

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
    if (isBoundary){
        boundaryBox.push_back(box);
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

/**
 * @brief Element::load Loading a model (.obj file)
 * @param filename The filename
 * @param isTexture Is texture supported
 * @return
 */
bool Element::load(const char *filename, bool isTexture){
    int val = 0;
    vector< vec3 > temp_vertices;
    vector< vec2 > temp_uvs;
    vector< vec3 > temp_normals;

    vec2 t;
    if (!isTexture)
    {
        t.x = 0;
        t.y = 0;
        temp_uvs.push_back(t);
        t.x = 0;
        t.y = 1;
        temp_uvs.push_back(t);
        t.x = 1;
        t.y = 0;
        temp_uvs.push_back(t);
        t.x = 1;
        t.y = 1;
        temp_uvs.push_back(t);
    }


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
            }else if ( strcmp( lineHeader, "vt" ) == 0 ){
                if (isTexture)
                {
                    vec2 uv;
                    val = fscanf(file, "%f %f\n", &uv.x, &uv.y );
                    temp_uvs.push_back(uv);
                }
            }else if ( strcmp( lineHeader, "vn" ) == 0 ){
                vec3 normal;
                val = fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
                temp_normals.push_back(normal);
            }else if ( strcmp( lineHeader, "f" ) == 0 ){
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                if (matches != 9){
                    //printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] );
                    if (matches != 6 ){
                        int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3] );
                        vertexIndices.push_back(vertexIndex[0]);
                        vertexIndices.push_back(vertexIndex[1]);
                        vertexIndices.push_back(vertexIndex[2]);
                        vertexIndices.push_back(vertexIndex[3]);
                        uvIndices    .push_back(uvIndex[0]);
                        uvIndices    .push_back(uvIndex[1]);
                        uvIndices    .push_back(uvIndex[2]);
                        uvIndices    .push_back(uvIndex[3]);
                        normalIndices.push_back(normalIndex[0]);
                        normalIndices.push_back(normalIndex[1]);
                        normalIndices.push_back(normalIndex[2]);
                        normalIndices.push_back(normalIndex[3]);
                    } else {
                        vertexIndices.push_back(vertexIndex[0]);
                        vertexIndices.push_back(vertexIndex[1]);
                        vertexIndices.push_back(vertexIndex[2]);
                        normalIndices.push_back(normalIndex[0]);
                        normalIndices.push_back(normalIndex[1]);
                        normalIndices.push_back(normalIndex[2]);
                    }

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
                //cerr << "Ignored line: " << ignore << val << endl;
            }

        }

        int k = 0;
        // For each vertex of each triangle
        for( unsigned int i=0; i<vertexIndices.size(); i++ ){
            // Get the indices of its attributes
            unsigned int vertexIndex = vertexIndices[i] -1;
            unsigned int uvIndex = 0;
            unsigned int normalIndex = normalIndices[i] -1;

            if (isTexture)
            {
                uvIndex = uvIndices[i] - 1;
            }
            else
            {
                uvIndex = k++;
            }

            // Get the attributes thanks to the index
            vec3 vertex = temp_vertices[ vertexIndex ];
            vec2 uv = temp_uvs[ uvIndex ];
            vec3 normal = temp_normals[ normalIndex ];

            // Put the attributes in buffers
            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);

            texCoords.push_back(uv.x);
            texCoords.push_back(uv.y);

            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);

            indices.push_back(i);

            if (k==4) {k = 0;}
        }
    }

    return true;
}

/**
 * @brief Element::calculate Calculating the center and the radius for each boundary box
 */
void Element::calculate(){

    //cout << "size: " << boundaryBox.size() << endl;
    for (unsigned int i=0; i<boundaryBox.size(); i++)
    {
        vec2 min, max;
        // init
        min.x = 50000.0f;
        min.y = 50000.0f;
        max.x = 0.0f;
        max.y = 0.0f;
        for (unsigned int j=0; j<((boundaryBox[i]).point).size(); j++) // for every element find the minimum coordinate and the maximum coordinate
        {
            //cout << "boundary " << i << " point " << j << ": " << ((boundaryBox[i]).point)[j].x << " " << ((boundaryBox[i]).point)[j].y << endl;

            // search for max
            if ( ((((boundaryBox[i]).point)[j]).x > max.x) || ((((boundaryBox[i]).point)[j]).y > max.y) )
            {
                max.x = (((boundaryBox[i]).point)[j]).x;
                max.y = (((boundaryBox[i]).point)[j]).y;
                //cout << "max: " << max.x << " " << max.y << endl;
            }

            // search for min
            if ( ((((boundaryBox[i]).point)[j]).x < min.x) || ((((boundaryBox[i]).point)[j]).y < min.y) )
            {
                min.x = (((boundaryBox[i]).point)[j]).x;
                min.y = (((boundaryBox[i]).point)[j]).y;
                //cout << "min: " << min.x << " " << min.y << endl;
            }
        }

        //cout << " ---------------------- " << endl;
        //cout << "min: " << min.x << " " << min.y << endl;
        //cout << "max: " << max.x << " " << max.y << endl;

        (boundaryBox[i]).center.x = (max.x + min.x) / 2;
        (boundaryBox[i]).center.y = (max.y + min.y) / 2;
        //cout << (boundaryBox[i]).center.x << " " << (boundaryBox[i]).center.y << endl;
        (boundaryBox[i]).R = sqrt( pow((max.x-(boundaryBox[i]).center.x),2) + pow((max.y-(boundaryBox[i]).center.y),2) *0.6f );

        //cout << (boundaryBox[i]).R << endl;
    }

}

/**
 * @brief Element::setBoundary Calculate the boundary for the models (player)
 */
void Element::setBoundary()
{
    vector<vec2> points;
    boundary box;

    vec2 max, min;
    min.x = 50000.0f;
    min.y = 50000.0f;
    max.x = 0.0f;
    max.y = 0.0f;

    // select the biggest and lowest coordinate from the vertex array
    for (unsigned int i=0; i<vertices.size(); i+=3){
        if (vertices[i]>max.x && vertices[i+1]>max.y)
        {
            max.x = vertices[i];
            max.y = vertices[i+1];
        }
        if (vertices[i]<min.x && vertices[i+1]<min.y)
        {
            min.x = vertices[i];
            min.y = vertices[i+1];
        }
    }

    points.push_back(min);
    points.push_back(max);

    boundaryBox.clear();

    box.point = points;
    boundaryBox.push_back(box);
}


/**
 * @brief Element::move Move the object (player) and collision test
 * @param x number of pixels
 * @param y number of pixels
 * @param z number of pixels
 * @param box a vector containing the vectors containing the boundary boxes (each vector for each object)
 * @param force is the movement forced (init move)
 * @return
 */
bool Element::move(GLfloat x, GLfloat y, GLfloat z, vector< vector<boundary> > box, bool force)
{
    if (!force)
        cout << "Moving..." << endl;

    GLfloat d;
    bool intersect = false;
    bool moved = false;

    // saving the original vertices
    vector<GLfloat> temp_vertices;
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        temp_vertices.push_back(vertices[i]);
    }

    // calculating the new coordinates for the player
    setBoundary();
    calculate();

    // check the map border
    for (unsigned int i = 0; i < vertices.size(); i+=3)
    {
        vertices[i] += x;
        vertices[i+1] += y;
        vertices[i+2] += z;
        if (vertices[i]<0 || vertices[i+1]<0)
        {
            intersect = true;
        }
    }

    // for every object
    for (unsigned int j=0; j < box.size(); j++)
    {
        // recalculate the coordinates
        setBoundary();
        calculate();

        // seach intersection for the whole array (for instance player vs walls)
        for (unsigned int i = 0; i < box[j].size(); i++)
        {
            //cout << "box: (" << (box[i]).center.x << "," << (box[i]).center.y << ")";

            // calculating the distance
            d = sqrt ( pow( (box[j][i]).center.x - (boundaryBox[0]).center.x, 2) + pow((box[j][i]).center.y - (boundaryBox[0]).center.y, 2));
            //cout << " d: " << d << " R1: " << box[i].R << " R2: " <<  (boundaryBox[0]).R <<  " R1+R2: " << (box[i].R + (boundaryBox[0]).R) << endl;

            // collision detected
            if (d < (box[j][i].R + (boundaryBox[0]).R) ){
                intersect = true;
                cout << "Collision detected! (" << (boundaryBox[0]).center.x << "," << (boundaryBox[0]).center.y << ") " << endl;
            }
        }
    }

    // if collision than roll back to the original vertices
    if (intersect && !force)
    {
        for (unsigned int i = 0; i < vertices.size(); i++)
        {
            vertices[i] = temp_vertices[i];
        }
    } else {
        // move the camera
        moved = true;
    }

    return moved;
}


vector<GLfloat> Element::getNormals() const
{
    return normals;
}

void Element::setNormals(const vector<GLfloat> &value)
{
    normals = value;
}

vector<GLfloat> Element::getTexCoords() const
{
    return texCoords;
}

void Element::setTexCoords(const vector<GLfloat> &value)
{
    texCoords = value;
}

vector<GLuint> Element::getIndices() const
{
    return indices;
}

void Element::setIndices(const vector<GLuint> &value)
{
    indices = value;
}

vector<GLfloat> Element::getVertices() const
{
    return vertices;
}

void Element::setVertices(const vector<GLfloat> &value)
{
    vertices = value;
}

int Element::getTextureId() const
{
    return textureId;
}

void Element::setTextureId(int value)
{
    textureId = value;
}

vector<boundary> Element::getBoundaryBox() const
{
    return boundaryBox;
}

void Element::setBoundaryBox(const vector<boundary> &value)
{
    boundaryBox = value;
}
