TEMPLATE    = app
QT         += opengl

INCLUDEPATH +=  /usr/include/glm /dades/florencia.rimolo/idi/lab/Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
MyGLWidget.cpp /dades/florencia.rimolo/idi/lab/Model/model.cpp

