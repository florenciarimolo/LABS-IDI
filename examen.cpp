// iniCamera

void MyEuler::inicialitzaCamera()
{
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

  obs = glm::vec3(0.0, 0.0, 1.5 * radi);
  vrp = glm::vec3(0.0, 0.0, 0.0);
  vup = glm::vec3(0.0, 1.0, 0.0);

  psi    = 0.0;  // gir exiY
  theta  = 0.0;  // gir eixX
  phi    = 0.0;  // gir eixZ. no s'aplica

  float d = 0;
  for (int i = 0; i < 3; i += 1){
    d = d + (obs[i] - vrp[i]) * (obs[i] - vrp[i]);
  }
  d     = sqrt(d);  // Com que vrp = (0,0,0), serà d = 1.5*radi
  znear = (d - radi) / 2.0;
  zfar  = d + radi;
  fovi  = 2.0 * asin(radi / d); // (float)M_PI / 2.0f;
  fov   = fovi;
  ra    = 1.0;
  deltaFov = (float)M_PI / 50.0f;
}

void MyEuler::radiEsferaContenidora()
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
  escala    = 1.0 / (ymax - ymin);

  centreBasePatricio[0] = centre[0];
  centreBasePatricio[1] = ymin;
  centreBasePatricio[2] = centre[2];

 // Escalat dels max/min del model
  xmax = xmax * escala;
  ymax = ymax * escala;
  zmax = zmax * escala;
  xmin = xmin * escala;
  ymin = ymin * escala;
  zmin = zmin * escala;
  
  // Inclusió del terra en el min/max
  if (xmax <  2.0) xmax =  2;
  if (xmin > -2.0) xmin = -2;
  if (zmax <  2.0) zmax =  2;
  if (zmin > -2.0) zmin = -2;

  // Càlcul del radi a partir del model
  float dx  = xmax - xmin;
  float dy  = ymax - ymin;
  float dz  = zmax - zmin;
  radi      = sqrt(dx * dx + dy * dy + dz * dz)/2.0;

  // Càlcul sabent les dimensions de l'escena final
  // -- un pèl matusser però l'escena queda més plena
  
  // radi = sqrt(4*4 + 4*4 + 1*1)/2.0;
}

void MyEuler::psiAngle (int angle)
{
  makeCurrent();
  psi = (float) angle / 360.0 * (float)M_PI;
  update ();
}

void MyEuler::thetaAngle (int angle)
{
  makeCurrent();
  theta = (float) angle / 360.0 * (float)M_PI;
  update ();
}



// ZOOM amb tecles

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

// ZOOM i angles d'Euler movent el ratolí

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
      int auxFov = (int) fov / (int) M_PI * 180;
      zoomChanged(auxFov);
    }
    update ();
    
    xClick = e->x();
    yClick = e->y();
}
