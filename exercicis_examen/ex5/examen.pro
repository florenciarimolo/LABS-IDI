TEMPLATE    = app
QT         += opengl

INCLUDEPATH +=  /usr/local/Cellar/glm/0.9.8.5/include
INCLUDEPATH += /Users/florenciarimolo/Dropbox/uni/idi/lab/exercicis_examen/examen/Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp  MyForm.cpp \
        MyGLWidget.cpp /Users/florenciarimolo/Dropbox/uni/idi/lab/exercicis_examen/examen/Model/model.cpp
