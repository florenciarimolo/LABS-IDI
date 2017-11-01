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
    // mouse events
    virtual void mousePressEvent   (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent    (QMouseEvent *event);
    // keyPressEvent - Es cridat quan es prem una tecla
    //virtual void keyPressEvent (QKeyEvent *event);

  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransformPatricioA ();
    void modelTransformPatricioB ();
    void modelTransformTerra ();
    void projectTransform ();
    void viewTransform ();
    void iniCamera ();
    void radiEsferaContenidora ();
    // Interaccio amb cursor
    typedef enum {NOINTERACCIO, ROTACIO} Interaccio;
    Interaccio interaccio;
    int        xClick, yClick;
    
    Model model;
    
    GLuint viewLoc;
    GLuint projectaLoc;
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc;
    // VAO i VBO names
    GLuint VAO_Terra, VBO_TerraVert, VBO_TerraColor;
    GLuint VAO_Model, VBO_ModelVert, VBO_ModelMat;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float scale;
    glm::vec3 pos, obs, vrp, up, centre, centreBase, v, eixX, eixY, eixZ;
    float lefti, righti, bottomi, left, right, bottom, ra, top, topi, znear, zfar;
    float radi, angle, theta, psi, phi, deltaA, deltaFov;
};

