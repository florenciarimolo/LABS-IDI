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
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 9);

  glBindVertexArray(0);
  // Activem l'Array a pintar
  glBindVertexArray(VAO2);

  // Pintem l'escena
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
  glm::vec3 Vertices[9];  // Nou vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(0.0, -1.0, 0.0);  // inf dret
  Vertices[1] = glm::vec3(-0.5, -1.0, 0.0); // inf esq
  Vertices[2] = glm::vec3(-0.5, -0.5, 0.0); // sup esq
  
  Vertices[3] = glm::vec3(0.0, -1.0, 0.0);  // inf dret
  Vertices[4] = glm::vec3(-0.5, -0.5, 0.0); // sup esq
  Vertices[5] = glm::vec3(0.0, -0.5, 0.0);  // sup dret

  Vertices[6] = glm::vec3(-0.25, -0.25, 0.0);  // punta
  Vertices[7] = glm::vec3(-0.5, -0.5, 0.0); // sup esq
  Vertices[8] = glm::vec3(0.0, -0.5, 0.0);  // sup dret

  glm::vec3 Vertices2[3];  // Tres vèrtexs amb X, Y i Z
  Vertices2[0] = glm::vec3(1.0, 0.75, 0.0);  //  sup
  Vertices2[1] = glm::vec3(0.75, -0.25, 0.0); // inf dret
  Vertices2[2] = glm::vec3(0.0, 0.0, 0.0); // inf esq
    
    
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  // Desactivem el VAO
  glBindVertexArray(0);
    
    
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);

  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("/Users/florenciarimolo/Dropbox/uni/idi/lab/bloc1/sessio1/ex4/shaders/fragshad.frag");
  vs.compileSourceFile("/Users/florenciarimolo/Dropbox/uni/idi/lab/bloc1/sessio1/ex4/shaders/vertshad.vert");
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
}
