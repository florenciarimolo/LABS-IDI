#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void createBuffers ();
    void carregaShaders ();
    void carregaModel ();
    void modelTransformHomer ();
    void modelTransformTerra();
    void projectTransform ();
    void viewTransform ();
    void iniCamera ();
    void carregaHomer ();
    void carregaTerra ();
    
    Model homer;
    
    GLuint viewLoc;
    GLuint projectaLoc;
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc;
    // VAO i VBO names
    GLuint VAO_Terra, VBO_TerraVert, VBO_TerraColor;
    GLuint VAO_Homer, VBO_HomerVert, VBO_HomerMat;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float scale, rotate;
    glm::vec3 pos, obs, vrp, up;
    float fovi, fov, ra, znear, zfar;
};

