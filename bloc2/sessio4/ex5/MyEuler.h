/*
 * euler amb QT  
 * RJA novembre 2016

 * MyEuler.h
 */

#define GLM_FORCE_RADIANS

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class MyEuler : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyEuler (QWidget *parent=0);
      ~MyEuler ();

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

    // mouse events
    virtual void mousePressEvent   (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent    (QMouseEvent *event);
    
  private:
    void createBuffers (std::string model_a_pintar);
    void carregaShaders ();
    void modelTransform_HA ();
    void modelTransform_HB ();
    void terraTransform ();
    void viewTransform ();
    void projectTransform ();
    void inicialitzaCamera ();
    void radiEsferaContenidora ();
    
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, viewLoc, projectaLoc;

    // VAO i VBO names
    GLuint VAO,       VBO_vertexs, VBO_color;
    GLuint VAO_terra, VBO_terraV,  VBO_terraC;

    // Program
    QOpenGLShaderProgram *program;

    // Model
    Model model;
    
    // Internal vars
    float escala;
    glm::vec3 pos;

    // Optica de la camera
    glm::vec3 obs, vrp, vup;
    glm::vec3 centre, centreBasePatricio;
    float fov, deltaFov, ra, znear, zfar;
    float fovi, rViewport;
    float radi;

    // angles d'Euler
    float theta, psi, phi;
    float maxAngle, deltaA;
    glm::vec3 eixX, eixY, eixZ;

    // Interaccio amb cursor
    typedef enum {NOINTERACCIO, ROTACIO} Interaccio;
    Interaccio interaccio;
    int        xClick, yClick;

    // Interacció amb widgets
    int width, height;

    public slots:
      void psiAngle (int angle);
      void thetaAngle (int angle);
      void fovAngle (int angle);
      void pintaPatricio();
    void pintaLegoman();
    
signals:
    
    void setPsi(int);
    void setTheta(int);
};
