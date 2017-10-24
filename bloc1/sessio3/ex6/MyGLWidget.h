#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    
    // interrupcions de teclat
    virtual void keyPressEvent (QKeyEvent *e);

  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform1 (); // pel primer obj
    void modelTransform2 (); // pel segon obj
    
    // attribute locations
    GLuint vertexLoc, transLoc;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO1, VBO1, VAO2, VBO2;
    float angle1, angle2;
};
