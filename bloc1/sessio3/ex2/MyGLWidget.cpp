#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  escala = 0.5;
  tx = ty = tz = 0.0;
  angle = 0.0;
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  glUniform1f(escalaLoc, escala);  // carregar valor inicial escala
  modelTransform();
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar 
  glBindVertexArray(VAO);
 
  // Pintem l'escena
  modelTransform();
  glDrawArrays(GL_TRIANGLES, 0, 3);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(1.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(0.0, 1.0, 0.0);

  glm::vec3 Colors[3];  //
  Colors[0] = glm::vec3(1.0, 0.0, 0.0);
  Colors[1] = glm::vec3(0.0, 1.0, 0.0);
  Colors[2] = glm::vec3(0.0, 0.0, 1.0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO_vert);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_vert);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("/Users/florenciarimolo/Dropbox/uni/idi/lab/bloc1/sessio3/ex2/shaders/fragshad.frag");
  vs.compileSourceFile("/Users/florenciarimolo/Dropbox/uni/idi/lab/bloc1/sessio3/ex2/shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");

  colorLoc = glGetAttribLocation (program->programId(), "color");

  escalaLoc = glGetUniformLocation(program->programId(), "escala");
    
  transLoc = glGetUniformLocation (program->programId(), "TG");
}

void MyGLWidget::modelTransform () {
    glm::mat4 TG (1.0); // Matriu de transformació, inicialment identitat
    TG = glm::translate (TG, glm::vec3 (tx, ty, tz));
    TG = glm::rotate (TG, angle, glm::vec3 (0., 0., 1.));
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::keyPressEvent (QKeyEvent *e)
{
    makeCurrent();
    switch ( e->key() )  {
        case Qt::Key_S :
            escala += 0.1;
            glUniform1f (escalaLoc, escala);
            break;
        case Qt::Key_D :
            escala -= 0.1;
            glUniform1f (escalaLoc, escala);
            break;
        case Qt::Key_Up :
            ty += 0.1;
            angle += M_PI/4;
            break;
        case Qt::Key_Down :
            ty -= 0.1;
            angle += M_PI/4;
            break;
        case Qt::Key_Left :
            tx -= 0.1;
            angle += M_PI/4;
            break;
        case Qt::Key_Right :
            tx += 0.1;
            angle += M_PI/4;
            break;
            
        default:
            e->ignore (); // propagar al pare
    }
    update();
}

