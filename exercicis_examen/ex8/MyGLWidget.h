#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
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
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event 
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    signals:

    void zoomChanged (int zoom);

    public slots:

    void setZoom(int zoom);

  private:
    void createBuffers ();
    void carregaShaders ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformTerra ();
    void modelTransformPatricioA ();
    //void modelTransformPatricioB ();
    void modelTransformVaca ();
    void calculaCapsaPatricio ();
    void calculaCapsaVaca ();
    void iniCamera ();
    void radiEsferaContenidora ();
    

    // VAO i VBO names
    GLuint VAO_Patr, VBO_PatrPos, VBO_PatrNorm, VBO_PatrMatamb, VBO_PatrMatdiff, VBO_PatrMatspec, VBO_PatrMatshin;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraNorm, VBO_TerraMatamb, VBO_TerraMatdiff, VBO_TerraMatspec, VBO_TerraMatshin;
    GLuint VAO_Vaca, VBO_VacaPos, VBO_VacaNorm, VBO_VacaMatamb, VBO_VacaMatdiff, VBO_VacaMatspec, VBO_VacaMatshin;
    // Program
    QOpenGLShaderProgram *program;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc,  pintaVacaLoc;

    // model
    Model patr, vaca;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centrePatr, centreVaca;
    float escalaV, escalaP;
    // radi de l'escena
    float radiEsc;
    float rotate;
    typedef  enum {NONE, ROTATE, ZOOM} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleY;
    int zoomAnt;
    bool perspectiva;
    glm::vec3 pos, obs, vrp, up, eixX, eixY, eixZ;
    float lefti, righti, bottomi, left, right, bottom, ra, top, topi, znear, zfar, fovi, fov;
    float  theta, psi, phi, deltaA, deltaFov;
};

