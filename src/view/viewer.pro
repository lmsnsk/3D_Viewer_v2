QT       += core gui opengl
LIBS += -lGLU

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    glview.cpp \
    main.cpp \
    mainwindow.cpp \
    ../modules/parser.cpp \
    ../modules/transforms.cpp \
    ../controller/controller.cpp

HEADERS += \
    glview.hpp \
    mainwindow.h \
    ../controller/controller.hpp \
    ../s21_3D_Viewer.hpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
