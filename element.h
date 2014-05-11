#ifndef GAMEELEMENT_H
#define GAMEELEMENT_H

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

#include <vector>
#include <iostream>
#include <string>
#include <string.h>
#include <cstdio>
#include <math.h>

using namespace std;

typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} vec3;

typedef struct {
    GLfloat x;
    GLfloat y;
} vec2;

typedef struct{
    vector<vec2> point;
    vec2 center;
    GLfloat R;
} boundary;


class Element {
private:
    vector<GLfloat> vertices;
    vector<GLuint> indices;
    vector<GLfloat> texCoords;
    vector<GLfloat> normals;
    vector<boundary> boundaryBox;
    int textureId;
public:
    Element();
    ~Element();

    void render(GLuint textures[]);
    bool move(GLfloat x, GLfloat y, GLfloat z, vector<boundary> box, bool force);
    void printVector(vector<GLfloat> value, int numberOfCoords);
    void printVector(vector<GLuint> value, int numberOfCoords);
    void GenerateSide(int coords[], int i, int j, int count, int orientation, float sizeX, float sizeY, float sizeZ, bool isBoundary);
    bool load(const char *filename);

    vector<GLfloat> getVertices() const;
    void setVertices(const vector<GLfloat> &value);
    vector<GLuint> getIndices() const;
    void setIndices(const vector<GLuint> &value);
    vector<GLfloat> getTexCoords() const;
    void setTexCoords(const vector<GLfloat> &value);
    vector<GLfloat> getNormals() const;
    void setNormals(const vector<GLfloat> &value);
    int getTextureId() const;
    void setTextureId(int value);
    void calculate();
    void setBoundary();
    vector<boundary> getBoundaryBox() const;
    void setBoundaryBox(const vector<boundary> &value);
};

#endif // GAMEELEMENT_H
