TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/local/Cellar/glm/0.9.8.5/include

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
        MyGLWidget.cpp 
