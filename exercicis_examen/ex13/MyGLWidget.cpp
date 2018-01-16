#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  angleY = 0.0;
  perspectiva = true;
  DoingInteractive = NONE;
  radiEsc = sqrt(5);
  deltaTrans = 0.1f;
  trans = glm::vec3(0,0,0);
    //escala = 1.0f;
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
  glEnable (GL_DEPTH_TEST);
  carregaShaders ();
  createBuffers ();
    iniCamera();
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    projectTransform();
    viewTransform();

  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);

  modelTransformTerra ();

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 12);

  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Patr);

  modelTransformPatricioA ();
    
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
    
    // Activem el VAO per a pintar el Patricio
    glBindVertexArray (VAO_Patr);
    
  modelTransformPatricioB ();

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
    float rViewport = float (w) / float (h);
    ra  = rViewport;
    if (perspectiva) {
        if (rViewport < 1.0) {
            fov = 2.0 * atan(tan(fovi/2.0)/rViewport);
        }
    }
    else {
        if (rViewport < 1.0) {
            top = radiEsc / rViewport;
            bottom = - radiEsc / rViewport;
            left = - radiEsc;
            right = radiEsc;
        }
        else {
            left = - radiEsc * rViewport;
            right = radiEsc * rViewport;
            top = radiEsc;
            bottom = - radiEsc;
        }
    }

    
    // Es conserva la vista. Mesures en pixels.
    glViewport(0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  patr.load("./models/Patricio.obj");

  // Calculem la capsa contenidora del model
  //calculaCapsaModel ();
  
  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Patr);
  glBindVertexArray(VAO_Patr);

  // Creació dels buffers del model patr
  // Buffer de posicions
  glGenBuffers(1, &VBO_PatrPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glGenBuffers(1, &VBO_PatrNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_PatrMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_PatrMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_PatrMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_PatrMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3, patr.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  // Dades del terra
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[12] = {
	glm::vec3(-2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(-2.0, 1.0, -2.0),
	glm::vec3(-2.0, 1.0, -2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(2.0, 1.0, -2.0)
  }; 

  // VBO amb la normal de cada vèrtex
  glm::vec3 norm1 (0,1,0);
  glm::vec3 norm2 (0,0,1);
  glm::vec3 normterra[12] = {
	norm1, norm1, norm1, norm1, norm1, norm1, // la normal (0,1,0) per als primers dos triangles
	norm2, norm2, norm2, norm2, norm2, norm2  // la normal (0,0,1) per als dos últims triangles
  };

  // Definim el material del terra
  glm::vec3 amb(0.2,0,0.2);
  glm::vec3 diff(0.8,0,0.8);
  glm::vec3 spec(0,0,0);
  float shin = 100;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambterra[12] = {
	amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffterra[12] = {
	diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspecterra[12] = {
	spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec
  };
  float matshinterra[12] = {
	shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_TerraNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_TerraMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambterra), matambterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_TerraMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffterra), matdiffterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_TerraMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspecterra), matspecterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_TerraMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshinterra), matshinterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
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
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::modelTransformPatricioA ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, trans);
  TG = glm::translate(TG, glm::vec3(-2, -1, 0));
  TG = glm::rotate(TG, (float)M_PI/2, glm::vec3(0, 1, 0));
  TG = glm::scale(TG, glm::vec3(escalaPA, escalaPA, escalaPA));
  TG = glm::translate(TG, -centrePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricioB ()
{
    glm::mat4 TG(1.f);  // Matriu de transformació
    TG = glm::translate(TG, glm::vec3(2, -1, 1));
    TG = glm::scale(TG, glm::vec3(escalaPB, escalaPB, escalaPB));
    TG = glm::rotate(TG, -(float)M_PI / 2, glm::vec3(0, 1, 0));
    TG = glm::translate(TG, -centrePatr);
    
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  if (perspectiva)
    Proj = glm::perspective(fov,ra,znear,zfar);
  else
    Proj = glm::ortho(left, right, bottom, top, znear, zfar);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
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

void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = patr.vertices()[0];
  miny = maxy = patr.vertices()[1];
  minz = maxz = patr.vertices()[2];
  for (unsigned int i = 3; i < patr.vertices().size(); i+=3)
  {
    if (patr.vertices()[i+0] < minx)
      minx = patr.vertices()[i+0];
    if (patr.vertices()[i+0] > maxx)
      maxx = patr.vertices()[i+0];
    if (patr.vertices()[i+1] < miny)
      miny = patr.vertices()[i+1];
    if (patr.vertices()[i+1] > maxy)
      maxy = patr.vertices()[i+1];
    if (patr.vertices()[i+2] < minz)
      minz = patr.vertices()[i+2];
    if (patr.vertices()[i+2] > maxz)
      maxz = patr.vertices()[i+2];
  }
  escalaPA = 1.0/(maxy-miny);
    escalaPB = 2.5/(maxy-miny);
  centrePatr[0] = (minx+maxx)/2.0; centrePatr[1] = miny; centrePatr[2] = (minz+maxz)/2.0;
    
    // Escalat dels max/min del model
    /*maxx = maxx * escalaPB;
    maxy = maxy * escalaPB;
    maxz = maxz * escalaPB;
    minx = minx * escalaPB;
    miny = miny * escalaPB;
    minz = minz * escalaPB;
    
    // Inclusió del terra en el min/max
    if (maxx <  2.0) maxx =  2;
    if (minx > -2.0) minx = -2;
    if (maxz <  2.0) maxz =  2;
    if (minz > -2.0) minz = -2;*/
    
    maxx = 2;
    minx = -2;
    maxy = 2.5;
    miny = -1;
    maxz = 2;
    minz = -2;
    
    // Càlcul del radi a partir del model
    
    float dx  = maxx - minx;
    float dy  = maxy - miny;
    float dz  = maxz - minz;
    radiEsc      = sqrt(dx * dx + dy * dy + dz * dz)/2.0;
}


void MyGLWidget::iniCamera()
{
    calculaCapsaModel();
    xClick     = 0;
    yClick     = 0;
    deltaA     = M_PI / 180.0;
    DoingInteractive = NONE;
    eixX   = glm::vec3(1.0, 0.0, 0.0);
    eixY   = glm::vec3(0.0, 1.0, 0.0);
    eixZ   = glm::vec3(0.0, 0.0, 1.0);
    theta  = 0.0;
    psi    = 0.0;
    phi    = 0.0;
    
    obs = glm::vec3(0.0, 0.0, 2.0 * radiEsc);
    vrp = glm::vec3(0.0, 0.0, 0.0);
    up = glm::vec3(0.0, 1.0, 0.0);
    
    float d = 0;
    for (int i = 0; i < 3; i += 1){
        d = d + (obs[i] - vrp[i]) * (obs[i] - vrp[i]);
    }
    d     = sqrt(d);
    znear = (d - radiEsc) / 2.0;
    zfar  = d + radiEsc;
    fovi  = 2.0 * asin(radiEsc / d); // (float)M_PI / 2.0f;
    fov   = fovi;
    lefti = left = bottom = bottomi = -radiEsc;
    right = righti = top = topi = radiEsc;
    ra    = 1.0;
    
    viewTransform ();
    projectTransform ();
}


void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_O: { // canvia òptica entre perspectiva i axonomètrica
      perspectiva = !perspectiva;
      break;
    }
        case Qt::Key_A: { // canvia òptica entre perspectiva i axonomètrica
        if (trans.x <= 3.9) trans.x += deltaTrans;
        
      break;
    }
        case Qt::Key_D: { // canvia òptica entre perspectiva i axonomètrica
            if (trans.x >= 0.1)
            trans.x -= deltaTrans;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
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


