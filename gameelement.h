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
#define DEBUG false

using namespace std;

class GameElement {
private:

    vector<GLfloat>* vertices;
    vector<GLuint>* indices;
    vector<GLfloat>* texCoords;
    vector<GLfloat>* normals;
    int textureId;

public:

    GameElement();
    ~GameElement();

    void render(GLuint *textures);

    int getTextureId() const;
    void setTextureId(int value);

    vector<GLfloat> *getVertices() const;
    void setVertices(vector<GLfloat> *value);

    vector<GLuint> *getIndices() const;
    void setIndices(vector<GLuint> *value);

    vector<GLfloat> *getTexCoords() const;
    void setTexCoords(vector<GLfloat> *value);

    vector<GLfloat> *getNormals() const;
    void setNormals(vector<GLfloat> *value);


};

#endif // GAMEELEMENT_H
