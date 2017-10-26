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
  modelTransformPatricio();

  // Activem el VAO per a pintar el model
  glBindVertexArray (VAO_Model);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, model.faces().size()*3);

  // Desactivem el VAO del model
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

void MyGLWidget::modelTransformPatricio ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, -centre);
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
    // Matriu de projecció del model: perspective(fov (en rad), ra, znear, zfar)
    glm::mat4 projecta = glm::perspective(fov,ra,znear,zfar);
    glUniformMatrix4fv(projectaLoc, 1, GL_FALSE, &projecta[0][0]);
}

void MyGLWidget::createBuffers () 
{
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
  fs.compileSourceFile("/Users/florenciarimolo/Dropbox/uni/idi/lab/bloc2/sessio2/ex6/shaders/fragshad.frag");
  vs.compileSourceFile("/Users/florenciarimolo/Dropbox/uni/idi/lab/bloc2/sessio2/ex6/shaders/vertshad.vert");
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
    v   = glm::vec3(0.0, 0.0, 1.0);  // v = vector normalitzat en qualsevol direcció
    
    vrp = glm::vec3(0.0, 0.0, 0.0);
    obs = vrp;   // OBS = VRP + d * v, on d > radi
    for (int i = 0; i < 3; i += 1){
        obs[i] += v[i] * d;
    }
    up = glm::vec3(0.0, 1.0, 0.0);
 
    znear = d - radi;
    zfar  = d + radi;
    fovi  = 2.0 * asin(radi / d); // (float)M_PI / 2.0f;
    fov   = fovi;
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
    float dx  = xmax - xmin;
    float dy  = ymax - ymin;
    float dz  = zmax - zmin;
    radi      = sqrt(dx * dx + dy * dy + dz * dz)/2.0;
    centre[0] = (xmax + xmin) / 2.0;
    centre[1] = (ymax + ymin) / 2.0;
    centre[2] = (zmax + zmin) / 2.0;
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

