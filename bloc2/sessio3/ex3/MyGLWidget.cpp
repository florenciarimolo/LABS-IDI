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
  iniCamera();
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
  // Càmera
  projectTransform();
  viewTransform();

  // Carreguem la transformació de model del Homer
  modelTransformPatricioA();

  // Activem el VAO per a pintar el model
  glBindVertexArray (VAO_Model);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, model.faces().size()*3);
  
  glBindVertexArray (0);
  
    // Carreguem la transformació de model del Homer
  modelTransformPatricioB();

  // Activem el VAO per a pintar el model
  glBindVertexArray (VAO_Model);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, model.faces().size()*3);

  // Desactivem el VAO del model
  glBindVertexArray (0);
  
  modelTransformTerra ();
  
    glBindVertexArray (VAO_Terra);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 6);
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
        //fov = 2.0 * atan(tan(fovi/2.0)/rViewport);
    }
    
    // Es conserva la vista. Mesures en pixels.
    glViewport(0, 0, w, h);
}

void MyGLWidget::modelTransformPatricioA ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(1,0,1));
  transform = glm::scale(transform, glm::vec3(scale, scale, scale));
  transform = glm::translate(transform, -centreBase);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformPatricioB ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(-1,0,-1));
  transform = glm::scale(transform, glm::vec3(scale, scale, scale));
  transform = glm::rotate(transform, (float)M_PI, glm::vec3(0,1,0));
  transform = glm::translate(transform, -centreBase);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::viewTransform ()
{
    // Matriu de projecció del model
    // glm::mat4 view = glm::lookAt(obs, vrp, vup);
    // Visió amb angles d'Euler
    glm::mat4 view(1.0f);
    view = glm::translate(view, -obs);     // posa la camera a l'origen
    view = glm::rotate(view, -psi,  eixY); // gira
    view = glm::rotate(view, theta, eixX);
    view = glm::rotate(view, phi,   eixZ);
    view = glm::translate(view, -vrp);      // trasllada el vrp a l'origen
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
}

void MyGLWidget::projectTransform ()
{
    // Matriu de projecció del model
    glm::mat4 projecta = glm::ortho(left, right, bottom, top, znear, zfar);
    glUniformMatrix4fv(projectaLoc, 1, GL_FALSE, &projecta[0][0]);
}

void MyGLWidget::createBuffers () 
{
    
        // TERRA
    glm::vec3 Vertices[6];  // 6 vèrtexs amb X, Y i Z
    Vertices[0] = glm::vec3(2.0, 0.0, -2.0);
    Vertices[1] = glm::vec3(-2.0, 0.0, -2.0);
    Vertices[2] = glm::vec3(-2.0, 0.0, 2.0);
    Vertices[3] = glm::vec3(-2.0, 0.0, 2.0);
    Vertices[4] = glm::vec3(2.0, 0.0, -2.0);
    Vertices[5] = glm::vec3(2.0, 0.0, 2.0);
        
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

    // PATRICIO
    model.load("/Users/florenciarimolo/Dropbox/uni/idi/lab/models/Patricio.obj");
    
    // Creació VAO
    glGenVertexArrays(1, &VAO_Model);
    glBindVertexArray(VAO_Model);
    
    // VBO vèrtexs
    glGenBuffers(1, &VBO_ModelVert);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelVert);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_vertices(), GL_STATIC_DRAW);
    
    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);
    
    glGenBuffers(1, &VBO_ModelMat);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ModelMat);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_matdiff(), GL_STATIC_DRAW);
    
    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);
    
    // Desactivem el VAO
    glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("/Users/florenciarimolo/Dropbox/uni/idi/lab/bloc2/sessio3/ex3/shaders/fragshad.frag");
  vs.compileSourceFile("/Users/florenciarimolo/Dropbox/uni/idi/lab/bloc2/sessio3/ex3/shaders/vertshad.vert");
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

void MyGLWidget::iniCamera()
{
    radiEsferaContenidora();
    deltaFov = (float)M_PI / 50.0;
    xClick     = 0;
    yClick     = 0;
    deltaA     = M_PI / 180.0;
    interaccio = NOINTERACCIO;
    eixX   = glm::vec3(1.0, 0.0, 0.0);
    eixY   = glm::vec3(0.0, 1.0, 0.0);
    eixZ   = glm::vec3(0.0, 0.0, 1.0);
    theta  = 0.0;
    psi    = 0.0;
    phi    = 0.0;
    
    float d = 2 * radi;   // distància de l'OBS al VRP
    v   = glm::vec3(0.f, 0.f, 1.f);  // v = vector normalitzat en qualsevol direcció
    
    vrp = glm::vec3(0.0, 0.0, 0.0);
    obs = vrp + d * v;   // OBS = VRP + d * v, on d > radi

    up = glm::vec3(0.0, 1.0, 0.0);
 
    znear = d - radi;
    zfar  = d + radi;
    left = bottom = -radi;
    right = top = radi;
    ra    = 1.0;
    angle = 0.0;
    
    viewTransform ();
    projectTransform ();
}

void MyGLWidget::radiEsferaContenidora()
{
    float xmin, xmax, ymin, ymax, zmin, zmax;
    
    xmin = xmax = model.vertices()[0];
    ymin = ymax = model.vertices()[1];
    zmin = zmax = model.vertices()[2];
    for (unsigned int i = 3; i < model.vertices().size(); i += 3)
    {
        if (model.vertices()[i+0] < xmin)
            xmin = model.vertices()[i+0];
        if (model.vertices()[i+0] > xmax)
            xmax = model.vertices()[i+0];
        if (model.vertices()[i+1] < ymin)
            ymin = model.vertices()[i+1];
        if (model.vertices()[i+1] > ymax)
            ymax = model.vertices()[i+1];
        if (model.vertices()[i+2] < zmin)
            zmin = model.vertices()[i+2];
        if (model.vertices()[i+2] > zmax)
            zmax = model.vertices()[i+2];
    }

    centre[0] = (xmax + xmin) / 2.0;
    centre[1] = (ymax + ymin) / 2.0;
    centre[2] = (zmax + zmin) / 2.0;
    
    scale = 1 / (ymax - ymin);
    
    centreBase[0] = centre[0];
    centreBase[1] = centre[1] - (ymax - ymin) / 2.0;   // = ymin
    centreBase[2] = centre[2];
    
    // Escalat dels max/min del model
    xmax = xmax * scale;
    ymax = ymax * scale;
    zmax = zmax * scale;
    xmin = xmin * scale;
    ymin = ymin * scale;
    zmin = zmin * scale;
    
    if (xmax < 2.0) xmax = 2.0;
    if (xmin > -2.0) xmin = -2.0;
    if (zmax < 2.0) zmax = 2.0;
    if (zmin > -2.0) zmin = -2.0;
    
    float dx  = xmax - xmin;
    float dy  = ymax - ymin;
    float dz  = zmax - zmin;
    radi      = sqrt(dx * dx + dy * dy + dz * dz)/2.0;
    
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
    makeCurrent();
    xClick = e->x();
    yClick = e->y();
    if (e->button() & Qt::LeftButton) {
        interaccio = ROTACIO;
    }else{
        interaccio = NOINTERACCIO;
    }
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *)
{
    makeCurrent();
    interaccio = NOINTERACCIO;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();
    int dx = abs(e->x() - xClick);
    int dy = abs(e->y() - yClick);
    
    if (dx > dy){   // gir "psi" respecte eixY
        if (e->x() > xClick){
            psi  += abs(e->x() - xClick) * deltaA;
        }else if (e->x() < xClick){
            psi  -= abs(e->x() - xClick) * deltaA;
        }
    }else{  // gir "theta" respecte eixX
        if (e->y() > yClick){
            theta -= abs(e->y() - yClick) * deltaA;
        }else if (e->y() < yClick){
            theta += abs(e->y() - yClick) * deltaA;
        }
    }
    update ();
    
    xClick = e->x();
    yClick = e->y();
}

/*void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Z: { // escalar a més gran
      if (fov < (float) M_PI) fov += deltaFov;
      break;
    }
    case Qt::Key_X: { // escalar a més petit
      if (fov > 0.0) fov -= deltaFov;
      break;
    }

    default: event->ignore(); break;
  }
  update();
}*/

