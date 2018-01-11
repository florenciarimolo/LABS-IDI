QT           += opengl

CONFIG       += qt
TEMPLATE      = app

DEPENDPATH   +=.
INCLUDEPATH  += /usr/local/Cellar/glm/0.9.8.5/include /Users/florenciarimolo/Dropbox/uni/idi/lab/Model

FORMS        += MyForm.ui

HEADERS      += MyForm.h \
MyEuler.h

SOURCES      += main.cpp    \
MyForm.cpp  \
MyEuler.cpp \
/Users/florenciarimolo/Dropbox/uni/idi/lab/Model/model.cpp

