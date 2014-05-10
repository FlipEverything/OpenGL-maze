#include "gameelement.h"

vector<GLfloat> *GameElement::getNormals() const
{
    return normals;
}

void GameElement::setNormals(vector<GLfloat> *value)
{
    normals = value;
}

vector<GLfloat> *GameElement::getTexCoords() const
{
    return texCoords;
}

void GameElement::setTexCoords(vector<GLfloat> *value)
{
    texCoords = value;
}

vector<GLuint> *GameElement::getIndices() const
{
    return indices;
}

void GameElement::setIndices(vector<GLuint> *value)
{
    indices = value;
}

vector<GLfloat> *GameElement::getVertices() const
{
    return vertices;
}

void GameElement::setVertices(vector<GLfloat> *value)
{
    vertices = value;
}


GameElement::GameElement()
{
    indices = new vector<GLuint>();
    vertices = new vector<GLfloat>();
    texCoords = new vector<GLfloat>();
    normals = new vector<GLfloat>();
    textureId = NULL;
}

GameElement::~GameElement()
{

}

void GameElement::render(GLuint *textures)
{
    // render the vertex array
    glBindTexture(GL_TEXTURE_2D, textures[textureId]); // texture data
    glVertexPointer(3, GL_FLOAT, 0, vertices->data()); // vertex
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords->data()); // texture coordinate
    glNormalPointer(GL_FLOAT, 0, normals->data()); // normalvector
    glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, indices->data()); // index
}



int GameElement::getTextureId() const
{
    return textureId;
}

void GameElement::setTextureId(int value)
{
    textureId = value;
}
