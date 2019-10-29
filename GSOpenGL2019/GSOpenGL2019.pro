QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++11

TARGET      = 3D-programmering

SOURCES += main.cpp \
    Npc.cpp \
    bsplinecurve.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    xyz.cpp \
    vector3d.cpp \
    graph.cpp \
    vertex.cpp \
    vector2d.cpp \
    visualobject.cpp \
    matrix4x4.cpp \
    vector4d.cpp \
    trianglesurface.cpp \
    camera.cpp \
    octahedronball.cpp \
    beziercurve.cpp \
    stb_image.cpp \
    material.cpp \
    textureshader.cpp \
    phongshader.cpp \
    light.cpp \
    texture.cpp \
    colorshader.cpp \
    objmesh.cpp \
    billboard.cpp \
    spriteshader.cpp \
    collisionaabb.cpp

HEADERS += \
    Npc.h \
    bsplinecurve.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    vertex.h \
    xyz.h \
    vector3d.h \
    graph.h \
    vector2d.h \
    visualobject.h \
    matrix4x4.h \
    vector4d.h \
    trianglesurface.h \
    camera.h \
    octahedronball.h \
    beziercurve.h \
    mathlab.h \
    stb_image.h \
    material.h \
    textureshader.h \
    phongshader.h \
    light.h \
    texture.h \
    colorshader.h \
    objmesh.h \
    billboard.h \
    spriteshader.h \
    collisionaabb.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    plainfragment.frag \
    plainvertex.vert \
    texturevertex.vert \
    texturefragment.frag \
    phong.frag \
    phong.vert \
    spritevertex.vert
