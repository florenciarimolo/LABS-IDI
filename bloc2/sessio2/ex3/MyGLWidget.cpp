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

  viewTransform();
  projectTransform();
  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_Homer);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, homer.faces().size()*3);

  glBindVertexArray (0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  // Es crida quan canvien les dimensions de la vista
  // els parametres (w, h) corresponen a la nova vista!!
  // Es fixa el ratio al nou valor i
  // S'ajusta la finestra (el fov), si cal

  float rViewport = float (w) / float (h);
  ra  = rViewport;
  if (rViewport < 1.0) {
    fov = 2.0 * atan(tan(fovi/2.0)/rViewport);
  }

  // Es conserva la vista. Mesures en pixels.
  glViewport(0, 0, w, h);
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
           // Matriu de transformaciÃ³ de model
       glm::mat4 transform (1.0f);
         transform = glm::scale(transform, glm::vec3(scale));
        transform = glm::rotate(transform, rotate, glm::vec3(0,1,0));
       transform = glm::translate(transform, -centre);
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
  glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
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
        radiEsferaContenidora();

        obs = glm::vec3(0.0, 0.0, 1.5 * radi);
        vrp = glm::vec3(0.0, 0.0, 0.0);
        up = glm::vec3(0.0, 1.0, 0.0);

        float d = 0;
        for (int i = 0; i < 3; i += 1){
          d = d + (obs[i] - vrp[i]) * (obs[i] - vrp[i]);
        }
        d     = sqrt(d);
        znear = (d - radi) / 2.0;
        zfar  = d + radi;
        fovi  = 2.0 * asin(radi / d); // (float)M_PI / 2.0f;
        fov   = fovi;
        ra    = 1.0;
        angle = 0.0;
  
        viewTransform ();
        projectTransform ();
}

void MyGLWidget::carregaModel(){
    homer.load("/dades/florencia.rimolo/idi/lab/models/Patricio.obj");
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

}

void MyGLWidget::radiEsferaContenidora()
{
       float xmin, xmax, ymin, ymax, zmin, zmax;
  
       xmin = xmax = homer.vertices()[0];
       ymin = ymax = homer.vertices()[1];
       zmin = zmax = homer.vertices()[2];
       for (unsigned int i = 3; i < homer.vertices().size(); i += 3)
       {
         if (homer.vertices()[i+0] < xmin)
           xmin = homer.vertices()[i+0];
         if (homer.vertices()[i+0] > xmax)
           xmax = homer.vertices()[i+0];
         if (homer.vertices()[i+1] < ymin)
           ymin = homer.vertices()[i+1];
         if (homer.vertices()[i+1] > ymax)
           ymax = homer.vertices()[i+1];
         if (homer.vertices()[i+2] < zmin)
           zmin = homer.vertices()[i+2];
         if (homer.vertices()[i+2] > zmax)
           zmax = homer.vertices()[i+2];
       }
       float dx  = xmax - xmin;
       float dy  = ymax - ymin;
       float dz  = zmax - zmin;
       radi      = sqrt(dx * dx + dy * dy + dz * dz)/2.0;
       centre[0] = (xmax + xmin) / 2.0;
       centre[1] = (ymax + ymin) / 2.0;
       centre[2] = (zmax + zmin) / 2.0; 
}

