#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
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

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  createBuffers();
  ini_camera();
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransform ();
  projectTransform();
  viewTransform();

  // Activem el VAO per a pintar el Homer
  glBindVertexArray (VAO_Homer);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, homer.faces().size()*3);

  glBindVertexArray (0);
  modelTransformTerra();
    // Activem el VAO per a pintar el terra
  glBindVertexArray (VAO_Terra);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray (0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport(0, 0, w, h);
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform, rotate, glm::vec3(0,1,0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::viewTransform ()
{
    // Matriu de projecciÃ³ del model. lookAt(obs, vrp, vup)
    glm::mat4 view = glm::lookAt(obs,vrp,up);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
}

void MyGLWidget::projectTransform ()
{
    // Matriu de projecciÃ³ del model: perspective(fov (en rad), ra, znear, zfar)
    glm::mat4 projecta = glm::perspective(fov,ra,znear,zfar);
    glUniformMatrix4fv(projectaLoc, 1, GL_FALSE, &projecta[0][0]);
}



void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
  case Qt::Key_R: { // rotem 45 graus
      rotate += M_PI/4;
    break;
  }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::createBuffers () 
{
  carregaModel();
  }

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("/Users/florenciarimolo/Dropbox/uni/idi/lab/bloc2/sessio1/ex6/shaders/fragshad.frag");
  vs.compileSourceFile("/Users/florenciarimolo/Dropbox/uni/idi/lab/bloc2/sessio1/ex6/shaders/vertshad.vert");
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
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projectaLoc = glGetUniformLocation(program->programId(), "PR");
    viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::ini_camera() {
    fov = M_PI/2.0;
    ra = 1.0;
    znear = 0.4;
    zfar = 3.0;

    obs = glm::vec3(0,0,1);
    vrp = glm::vec3(0,0,0);
    up = glm::vec3(0,1,0);

    projectTransform();
    viewTransform();
}

void MyGLWidget::carregaModel(){
    
    // TERRA
  glm::vec3 Vertices[6];  // 6 vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-1.0, -1.0, 1.0);
  Vertices[1] = glm::vec3(1.0, -1.0, 1.0);
  Vertices[2] = glm::vec3(1.0, -1.0, -1.0);
  Vertices[3] = glm::vec3(-1.0, -1.0, 1.0);
  Vertices[4] = glm::vec3(-1.0, -1.0, -1.0);
  Vertices[5] = glm::vec3(1.0, -1.0, -1.0);
    
  glm::vec3 Colors[6];   // color del terra
  Colors[0] = glm::vec3(1.0, 1.0, 0.0);
    Colors[1] = glm::vec3(1.0, 1.0, 0.0);
    Colors[2] = glm::vec3(1.0, 1.0, 0.0);
    Colors[3] = glm::vec3(1.0, 1.0, 0.0);
    Colors[4] = glm::vec3(1.0, 1.0, 0.0);
    Colors[5] = glm::vec3(1.0, 1.0, 0.0);
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO_TerraVert);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraVert);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
    // Creació del buffer amb les dades dels vèrtexs
    glGenBuffers(1, &VBO_TerraColor);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraColor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
    // Activem l'atribut que farem servir per vèrtex
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);
  // Desactivem el VAO
  glBindVertexArray(0);
    
    // HOMER
    homer.load("/Users/florenciarimolo/Dropbox/uni/idi/lab/models/HomerProves.obj");
    glGenVertexArrays(1, &VAO_Homer);
    glBindVertexArray(VAO_Homer);

    glGenBuffers(1, &VBO_HomerVert);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerVert);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3, homer.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &VBO_HomerMat);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerMat);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3, homer.VBO_matdiff(), GL_STATIC_DRAW);
    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);
    // Desactivem el VAO
    glBindVertexArray (0);



}

