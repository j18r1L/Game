QT -= gui
QT += core
CONFIG += c++11

###############linux###############

unix: LIBS += -L/usr/lib/ -lGL

#glew
unix: LIBS += -L/usr/lib/x86-x64-linux-gnu/ -lGLEW
unix: INCLUDEPATH += /usr/include/GL
unix: DEPENDPATH += /usr/include/GL

#glfw
unix: LIBS += -L/usr/lib/x86-x64-linux-gnu/ -lglfw
unix: INCLUDEPATH += /usr/include/GLFW
unix: DEPENDPATH += /usr/include/GLFW

#glm
unix: INCLUDEPATH += $$PWD/../Lib
unix: DEPENDPATH += $$PWD/../Lib

#stb image
unix: INCLUDEPATH += $$PWD/../Lib/stb_image
unix: DEPENDPATH += $$PWD/../Lib/stb_image

#assimp
unix: INCLUDEPATH += $$PWD/../Lib/linux/include
unix: DEPENDPATH += $$PWD/../Lib/linux/include

unix: LIBS += -L$$PWD/../Lib/linux/lib/ -lassimp
unix: LIBS += -L$$PWD/../Lib/linux/lib/ -lIrrXML
unix: LIBS += -L$$PWD/../Lib/linux/lib/ -lminizip
unix: LIBS += -L$$PWD/../Lib/linux/lib/ -lz

unix: PRE_TARGETDEPS += $$PWD/../Lib/linux/lib/libassimp.a
unix: PRE_TARGETDEPS += $$PWD/../Lib/linux/lib/libIrrXML.a
unix: PRE_TARGETDEPS += $$PWD/../Lib/linux/lib/libminizip.a
unix: PRE_TARGETDEPS += $$PWD/../Lib/linux/lib/libz.a




###############MACOS###############

#glew
macx: LIBS += -L/usr/local/Cellar/glew/2.1.0_1/lib/ -lGLEW.2.1.0
macx:INCLUDEPATH += /usr/local/Cellar/glew/2.1.0_1/include
macx:DEPENDPATH += /usr/local/Cellar/glew/2.1.0_1/include

#assimp
macx: LIBS += -L/usr/local/Cellar/assimp/5.0.1/lib/ -lassimp.5.0.0
macx: INCLUDEPATH += /usr/local/Cellar/assimp/5.0.1/include
macx: DEPENDPATH += /usr/local/Cellar/assimp/5.0.1/include

#glfw
macx: LIBS += -L/usr/local/Cellar/glfw/3.3.2/lib/ -lglfw.3.3
macx: INCLUDEPATH += /usr/local/Cellar/glfw/3.3.2/include
macx: DEPENDPATH += /usr/local/Cellar/glfw/3.3.2/include

#glm
macx: INCLUDEPATH += /usr/local/Cellar/glm/0.9.9.5/include
macx: DEPENDPATH += /usr/local/Cellar/glm/0.9.9.5/include

macx: LIBS += -framework OpenGL


SOURCES += \
    ../Lib/stb_image/stb_image_aug.c \
    Camera.cpp \
    Components/CameraComponents.cpp \
    GameObject.cpp \
    Components/GraphicsComponents.cpp \
    Components/InputComponents.cpp \
    Mesh.cpp \
    Model.cpp \
    Components/ModelComponents.cpp \
    ObjectFabric.cpp \
    Components/PhysicsComponents.cpp \
    Renderer.cpp \
    Shader.cpp \
    VideoSettings.cpp \
    Window.cpp \
    main.cpp

HEADERS += \
    ../Lib/stb_image/stb_image_aug.h \
    ../Lib/stb_image/stbi_DDS_aug.h \
    ../Lib/stb_image/stbi_DDS_aug_c.h \
    Camera.h \
    Components/CameraComponents.h \
    GameObject.h \
    Components/GraphicsComponents.h \
    Components/InputComponents.h \
    Mesh.h \
    Model.h \
    Components/ModelComponents.h \
    ObjectFabric.h \
    Components/PhysicsComponents.h \
    Renderer.h \
    Shader.h \
    VideoSettings.h \
    Window.h

DISTFILES += \
    ../shaders/Model.frag \
    ../shaders/Model.vert \
    ../shaders/Editor.frag \
    ../shaders/Editor.vert


