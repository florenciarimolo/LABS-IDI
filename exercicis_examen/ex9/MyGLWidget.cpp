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
  rotate = 0;
  canviFranges = 0;
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
  glUniform1i(pintaVacaLoc, 0);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 12);


  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Patr);
    modelTransformPatricioA ();
    glUniform1i(pintaVacaLoc, 0);

    
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  

    
    // Activem el VAO per a pintar la vaca
    glBindVertexArray (VAO_Vaca);
    
    modelTransformVaca ();
    glUniform1i(pintaVacaLoc, 1);

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, vaca.faces().size()*3);
  
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
  
    // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  vaca.load("./models/cow.obj");

  // Calculem la capsa contenidora del model
  //calculaCapsaModel ();
  
  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Vaca);
  glBindVertexArray(VAO_Vaca);

  // Creació dels buffers del model patr
  // Buffer de posicions
  glGenBuffers(1, &VBO_VacaPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glGenBuffers(1, &VBO_VacaNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_VacaMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_VacaMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_VacaMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_VacaMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3, vaca.VBO_matshin(), GL_STATIC_DRAW);

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
    pintaVacaLoc = glGetUniformLocation (program->programId(), "pintaVaca");
      canviFrangesLoc = glGetUniformLocation (program->programId(), "canviFranges");
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::modelTransformPatricioA ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(1,-0.5,0));
  TG = glm::rotate(TG, rotate, glm::vec3(0, 1, 0));
  TG = glm::scale(TG, glm::vec3(escalaP, escalaP, escalaP));
  TG = glm::translate(TG, -centrePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformVaca ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(1,-1,0));
  TG = glm::scale(TG, glm::vec3(escalaV, escalaV, escalaV));
  TG = glm::rotate(TG, rotate, glm::vec3(0, 1, 0));
  TG = glm::rotate(TG, -(float)M_PI/2, glm::vec3(1, 0, 0));
  TG = glm::rotate(TG, -(float)M_PI/2, glm::vec3(0, 0, 1));
  TG = glm::translate(TG, -centreVaca);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

/*void MyGLWidget::modelTransformPatricioB ()
{
    glm::mat4 TG(1.f);  // Matriu de transformació
    TG = glm::translate(TG, glm::vec3(0, 2, 0));
    TG = glm::scale(TG, glm::vec3(escala, escala, escala));
    TG = glm::rotate(TG, (float)M_PI, glm::vec3(0, 0, 1));
    TG = glm::translate(TG, -centrePatr);
    
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}*/

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

void MyGLWidget::calculaCapsaPatricio ()
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
  escalaP = 0.25/(maxy-miny);
  centrePatr[0] = (minx+maxx)/2.0; centrePatr[1] = (miny+maxy)/2.0; centrePatr[2] = (minz+maxz)/2.0;
}

void MyGLWidget::calculaCapsaVaca ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = vaca.vertices()[0];
  miny = maxy = vaca.vertices()[1];
  minz = maxz = vaca.vertices()[2];
  for (unsigned int i = 3; i < vaca.vertices().size(); i+=3)
  {
    if (vaca.vertices()[i+0] < minx)
      minx = vaca.vertices()[i+0];
    if (vaca.vertices()[i+0] > maxx)
      maxx = vaca.vertices()[i+0];
    if (vaca.vertices()[i+1] < miny)
      miny = vaca.vertices()[i+1];
    if (vaca.vertices()[i+1] > maxy)
      maxy = vaca.vertices()[i+1];
    if (vaca.vertices()[i+2] < minz)
      minz = vaca.vertices()[i+2];
    if (vaca.vertices()[i+2] > maxz)
      maxz = vaca.vertices()[i+2];
  }
  escalaV = 0.5/(maxz-minz);
  centreVaca[0] = (minx+maxx)/2.0; centreVaca[1] = (miny+maxy)/2.0; centreVaca[2] = minz;
}

void MyGLWidget::radiEsferaContenidora ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
    
    // Escalat dels max/min del model
    maxx = 2;
    minx = -2;
    miny = -1;
    maxy = 1;
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
    calculaCapsaPatricio();
    calculaCapsaVaca();
    radiEsferaContenidora();
    deltaFov = (float)M_PI / 50.0;
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
    zoomAnt = fov;
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
      projectTransform ();
      break;
    }
    case Qt::Key_R: { // rotem 30 graus
      rotate += M_PI/6;
    break;
   }
    case Qt::Key_X: { // rotem 30 graus
      glUniform1i(canviFrangesLoc, canviFranges);
      if (canviFranges == 1) canviFranges = 0;
      else canviFranges = 1;
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
  else if (e->button() & Qt::RightButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ZOOM;
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
    if (DoingInteractive == ROTATE) {
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
    }
    if (DoingInteractive == ZOOM) {
      float FOVauxiliar = fov + (e->y() - yClick)/10.0;
      if(FOVauxiliar < (float) M_PI && FOVauxiliar > 0.0) fov= FOVauxiliar;

      float auxFov =  fov /  (float) M_PI * 180.0;
      emit zoomChanged((int) auxFov);
    }
    update ();
    
    xClick = e->x();
    yClick = e->y();
}

void MyGLWidget::setZoom(int zoom) {
  makeCurrent();
  if (zoom < 180 and zoom > 0) fov = zoom * (float) M_PI / 180.0;
  update();
}


