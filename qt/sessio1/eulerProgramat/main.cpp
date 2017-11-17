
/*
 * euler amb QT  
 * RJA novembre 2016

 * main.cpp
 */

#include <QApplication>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>

#include <QFrame>
#include <QLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QOpenGLWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

#include "MyEuler.h"

int main (int argc, char **argv) 
{
  QApplication app (argc, argv);
  QSurfaceFormat f;
  f.setVersion(3,3);
  f.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(f);

  int width  = 900;
  int height = 600;

  // Creació de la interfície gràfica
  QFrame *w = new QFrame();

  QFont font("Times", 15, QFont::Bold, true); 

  // MyEuler widget (layout)
  MyEuler *myEuler = new MyEuler(w);
  myEuler->setMinimumWidth(height);
  myEuler->setMinimumHeight(height);
 
  // control layout
  QLabel *labelPsi   = new QLabel("Gir eix Y");

  QLabel *labelTheta = new QLabel("Gir eix X");
  
  QSlider *psiSlider = new QSlider(Qt::Horizontal);
  psiSlider->setRange(-180, 180);

  QSlider *thetaSlider = new QSlider(Qt::Horizontal);  
  thetaSlider->setRange(-180, 180);

  QPushButton *sortirButton = new QPushButton("&Sortir");
  sortirButton->setStyleSheet("background-color:red;");
  sortirButton->setMinimumWidth(width/10);
  sortirButton->setMinimumHeight(height/10);

  QSpacerItem *quitSpacer = new QSpacerItem(10, height/2, // w, h
				   	    QSizePolicy::Expanding,
					    QSizePolicy::Minimum);  

  QVBoxLayout *controlLayout = new QVBoxLayout();
  controlLayout->addWidget(labelPsi);
  controlLayout->addWidget(psiSlider);
  controlLayout->addWidget(labelTheta);
  controlLayout->addWidget(thetaSlider);
  controlLayout->addItem(quitSpacer);
  controlLayout->addWidget(sortirButton);

  // final layout
  QSpacerItem *horSpacer = new QSpacerItem(30, height, // w, h
			       	           QSizePolicy::Expanding,
					   QSizePolicy::Minimum);

  QHBoxLayout *mainLayout = new QHBoxLayout (w);
  mainLayout->addWidget(myEuler);
  mainLayout->addItem(horSpacer); 
  mainLayout->addLayout(controlLayout);
  
  // Connexions signals -> slots
  QWidget::connect(psiSlider,   SIGNAL(valueChanged(int)),
    	           myEuler,     SLOT(psiAngle(int)));
  QWidget::connect(thetaSlider, SIGNAL(valueChanged(int)),
    	           myEuler,     SLOT(thetaAngle(int)));
  QWidget::connect(sortirButton, SIGNAL(clicked()),
                   w,            SLOT(close()));
  
  // execució
  w->resize(width, height);
  w->show();

  return app.exec ();
}
