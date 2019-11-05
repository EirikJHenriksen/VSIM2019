#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <vector>
#include <cmath>
#include "mathlab.h"
#include "vertex.h"

#include "texture.h"

#include "shader.h"
#include "colorshader.h"
#include "textureshader.h"
#include "phongshader.h"
#include "spriteshader.h"

#include "visualobject.h"
#include "camera.h"
#include "xyz.h"
#include "graph.h"
#include "beziercurve.h"
#include "trianglesurface.h"
#include "objmesh.h"
#include "billboard.h"
#include "octahedronball.h"
#include "light.h"
#include "bsplinecurve.h"

class Npc;
class QOpenGLContext;
class Shader;
class MainWindow;

/// This inherits from QWindow to get access to the Qt functionality and
/// OpenGL surface.
/// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
/// This is the same as using glad and glw from general OpenGL tutorials

typedef gs2019::Vector2D Vec2;
typedef gs2019::Vector3D Vec3;
typedef gs2019::Vector4D Vec4;
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;

    //    void error(const QString &msg);

    void toggleDebugItems();
    void toggleWireframe();

    void smoothTerrain();

    void resetPlayer();

    int currentScene = 1;
    bool timeActive = true;

    int cameraMode = 1;

    // ================ PLAYER BALL/PHYSICS BALL ===================
    Vec3 playerStartPos{1.f, 10.f, 0.5f};

private slots:
    void render();

private:
    void init();

    Npc* FiendeStatemachine{nullptr};

    //Camera
    Camera* TheCamera;

    Vec3 CamLoc01;
    Vec3 CamLoc02;
    Vec3 CamLoc03;
    Vec3 CamLoc04;
    Vec3 CamLoc05;

    float aspectRatio = 0.f;

    float fieldOfView = 90.f;
    bool cameraInHouse = false;

    std::vector<VisualObject*> DebugObjects;
    bool debug{true};

    std::vector<VisualObject*> SceneObjects;
    std::vector<VisualObject*> SceneObjects2;
    std::vector<VisualObject*> SceneObjects3;

    GLfloat time = 0.f;

    // Important/dynamic objects:
    VisualObject* interObj = nullptr;
    VisualObject* impactObj = nullptr;
    BezierCurve* path = nullptr;
    graph* terrainObj = nullptr;


    // Important/dynamic objects (VSIM):
    BSplineCurve* TheBSpline;

    VisualObject* mSpiller{nullptr};
    VisualObject* mFiende{nullptr};

    Vec3 mStartpunkt;
    Vec3 mSluttpunkt;

    VisualObject* mStjerne01{nullptr};
    VisualObject* mStjerne02{nullptr};
    VisualObject* mStjerne03{nullptr};
    VisualObject* mStjerne04{nullptr};

    // Rendering
    bool displayWireframe = false;

    QOpenGLContext *mContext;
    bool mInitialized;

    Shader* mShaderProgram[5]{nullptr};

    Texture* mTexture[10]{nullptr};

    GLint  mMatrixUniform{-1};
    GLint  mPMatrixUniform{-1};
    GLint  mVMatrixUniform{-1};

    GLuint mVAO;
    GLuint mVBO;

    //QMatrix4x4 *mMVPmatrix; //The matrix with the transform for the object we draw

    QMatrix4x4* mMatrix;
    QMatrix4x4* mVMatrix;
    QMatrix4x4* mPMatrix;

    QTimer *mRenderTimer;     //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the actual FPS

    MainWindow *mMainWindow;    //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

protected:
    //The QWindow that we inherit from has these functions to capture
    // mouse and keyboard. Uncomment to use
    //
    //    void mousePressEvent(QMouseEvent *event) override{}
    //    void mouseMoveEvent(QMouseEvent *event) override{}

    QSet<int> pressedKeys;
    void processInputs();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    //    void keyReleaseEvent(QKeyEvent *event) override{}
    //    void wheelEvent(QWheelEvent *event) override{}
};

#endif // RENDERWINDOW_H
