#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include "shader.h"
#include "mainwindow.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)
{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //This is the matrix used to transform the triangle
    //You could do without, but then you have to simplify the shader and shader setup
    //mMVPmatrix = new QMatrix4x4{};
    //mMVPmatrix->setToIdentity();

    mMatrix = new QMatrix4x4;
    mVMatrix = new QMatrix4x4;
    mPMatrix = new QMatrix4x4;

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);

}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

/// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Connect the gameloop timer to the render function:
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
    {
        mInitialized = true;
    }
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //==============================================================================================
    // ++++++++++++++++++++++++++++++++++++ RENDER SETTINGS ++++++++++++++++++++++++++++++++++++++++++++
    //==============================================================================================

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);    //enables depth sorting - must use GL_DEPTH_BUFFER_BIT in glClear
    glEnable(GL_CULL_FACE);     //draws only front side of models - usually what you want -
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);    //color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.

    mShaderProgram[0] = new ColorShader("../GSOpenGL2019/plainvertex.vert", "../GSOpenGL2019/plainfragment.frag");
    mShaderProgram[1] = new TextureShader("../GSOpenGL2019/texturevertex.vert", "../GSOpenGL2019/texturefragment.frag");
    mShaderProgram[2] = new PhongShader("../GSOpenGL2019/phong.vert", "../GSOpenGL2019/phong.frag");
    mShaderProgram[3] = new SpriteShader("../GSOpenGL2019/spritevertex.vert", "../GSOpenGL2019/texturefragment.frag");

    //-------------------------------------------------------------------------------------------
    //====================== Objects to initialize ======================
    //-------------------------------------------------------------------------------------------

    VisualObject* OrientationWidget = new XYZ();


    // =====================================================
    // +++++++++++++++++++++++| BASE |++++++++++++++++++++++++
    // =====================================================

    //XYZ-akser.
    DebugObjects.push_back(OrientationWidget);


    // =====================================================
    // ++++++++++++++++++++|Makes Textures|+++++++++++++++++++++
    // =====================================================

    mTexture[0] = new Texture("../GSOpenGL2019/Data/Textures/blank.gif", 0);
    mTexture[1] = new Texture("../GSOpenGL2019/Data/Textures/Trollface.png", 1);
    mTexture[2] = new Texture("../GSOpenGL2019/Data/Textures/Trollface.png", 2);
    mTexture[3] = new Texture("../GSOpenGL2019/Data/Textures/GameSheet.gif", 3);
    mTexture[4] = new Texture("../GSOpenGL2019/Data/Textures/Doughnut.png", 4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture[0]->id());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTexture[1]->id());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mTexture[2]->id());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mTexture[3]->id());

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, mTexture[4]->id());

    // =====================================================
    // ++++++++++++++++++++|Makes objects|++++++++++++++++++++++
    // =====================================================
    //VisualObject* Ground = new TriangleSurface("Ground", "../GSOpenGL2019/Data/Models/Ground.nei", Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false, false);
    //VisualObject* TheWall = new TriangleSurface("Wall_01", "../GSOpenGL2019/Data/Models/TheWall.nei", Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false, false);

    //VisualObject* WallLeft = new TriangleSurface("Box", "../GSOpenGL2019/Data/Models/Cube.nei", Vec3(10.5f, -2.f, 0.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 5.f, 20.f), false, false, true);
    //VisualObject* WallRight = new TriangleSurface("Box", "../GSOpenGL2019/Data/Models/Cube.nei", Vec3(-10.5f, -2.f, 0.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 5.f, 20.f), false, false, true);
    //VisualObject* WallTop = new TriangleSurface("Box", "../GSOpenGL2019/Data/Models/Cube.nei", Vec3(-1.f, -2.f, -10.5f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(18.f, 5.f, 1.f), false, false, true);
    //VisualObject* WallBottom = new TriangleSurface("Box", "../GSOpenGL2019/Data/Models/Cube.nei", Vec3(0.f, -2.f, 10.5f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(20.f, 5.f, 1.f), false, false, true);


    //VisualObject* TheOtherWall = new TriangleSurface("Wall_02", "../GSOpenGL2019/Data/Models/TheOtherWall.nei", Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false, false);
    //VisualObject* Building = new TriangleSurface("House", "../GSOpenGL2019/Data/Models/Building.nei", Vec3(6.f, 0.f, -4.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false, false);
    VisualObject* Door = new TriangleSurface("House_Door", "../GSOpenGL2019/Data/Models/Door.nei", Vec3(5.f, 0.f, -1.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false, false);
    //VisualObject* Tree1 = new TriangleSurface("Tree_01", "../GSOpenGL2019/Data/Models/Tree.nei", Vec3(-8.f, 0.f, 0.f), Vec3(0.f, 0.f, 0.f), 90.f, Vec3(2.f, 2.f, 2.f), false, false, true);
    //VisualObject* Tree2 = new TriangleSurface("Tree_02", "../GSOpenGL2019/Data/Models/Tree.nei", Vec3(-6.f, 0.f, -6.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(3.f, 3.f, 3.f), false, false, true);
    //VisualObject* Tree3 = new TriangleSurface("Tree_03", "../GSOpenGL2019/Data/Models/Tree.nei", Vec3(-1.f, 0.f, -7.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.5f, 1.5f, 1.5f), false, false, true);
    //VisualObject* Tree4 = new TriangleSurface("Tree_04", "../GSOpenGL2019/Data/Models/Tree.nei", Vec3(8.f, 0.f, 8.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.5f, 1.5f, 1.5f), false, false, true);
    //VisualObject* Tree5 = new TriangleSurface("Tree_05", "../GSOpenGL2019/Data/Models/Tree.nei", Vec3(-8.f, 0.f, 7.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(2.f, 2.f, 2.f), false, false, true);
    //VisualObject* Flag = new TriangleSurface("Flag", "../GSOpenGL2019/Data/Models/Flag.nei", Vec3(0.f, 0.f, -5.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false, true);


    trophyLoc01 = Vec3(-5.f, 0.f, 5.f);
    trophyLoc02 = Vec3(2.f, 0.f, -8.f);
    trophyLoc03 = Vec3(-7.f, 0.f, -7.f);
    trophyLoc04 = Vec3(9.f, 0.f, 0.f);

    VisualObject* Box1 = new TriangleSurface("Box", "../GSOpenGL2019/Data/Models/Cube.nei", trophyLoc01, Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), true, false, false);
    VisualObject* Box2 = new TriangleSurface("Box", "../GSOpenGL2019/Data/Models/Cube.nei", trophyLoc02, Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), true, false, false);
    VisualObject* Box3 = new TriangleSurface("Box", "../GSOpenGL2019/Data/Models/Cube.nei", trophyLoc03, Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), true, false, false);
    VisualObject* Box4 = new TriangleSurface("Box", "../GSOpenGL2019/Data/Models/Cube.nei", trophyLoc04, Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), true, false, false);

    VisualObject* Gate = new TriangleSurface("Gate", "../GSOpenGL2019/Data/Models/Plane.nei", Vec3(9.f, 0.5f, -10.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false, true);

    VisualObject* Terreng = new TriangleSurface("Gate", "../GSOpenGL2019/Data/Models/landskap.nei", Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false, false);

    // ================ TERRAIN GRAPH ================
    graph* TerrainGraph = new graph(true);
    // =======================================================

    BezierCurve* Curve = new BezierCurve(trophyLoc03, trophyLoc01, trophyLoc02, trophyLoc04, 3, 100, true, true);
    VisualObject* ThePyramid = new TriangleSurface("Moving_Pyramid", "../GSOpenGL2019/Data/Models/Pyramid.nei", Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false, false);
    DebugObjects.push_back(Curve);

    //VisualObject* IndoorsTetrahedron= new TriangleSurface("Tetrahedron", "../GSOpenGL2019/Data/Models/Tetrahedron.nei", Vec3(6.f, 0.f, -4.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), true, true, false);

    // ================ PLAYER BALL/PHYSICS BALL ===================
    VisualObject* Ball = new OctahedronBall("Player", 4, playerStartPos, Vec3(0.f, 0.f, 0.f), 0.f, Vec3(0.5f, 0.5f, 0.5f), false, false);
    // =======================================================

    VisualObject* Ball2 = new OctahedronBall("Small_Ball", 4, Vec3(3.f, -0.3f, 0.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(0.2f, 0.2f, 0.2f), false, false);

    // OBJ-Objects
    //VisualObject* Doughnut = new ObjMesh("Doughnut", "../GSOpenGL2019/Data/Models/Doughnut.obj", Vec3(3.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 10.f, Vec3(1.f, 1.f, 1.f), true, true);

    // Billboard
    VisualObject* ScoreBoard = new Billboard("Scoreboard", Vec3(0.f, 3.f, -10.f), Vec3(3.f, 3.f, 3.f));


    // ======================
    // -------------|Lights|-------------
    // ======================
    VisualObject* lightSrc01 = new Light(Vec3(10.f, 5.f, 10.f));
    static_cast<Light*>(lightSrc01)->mAmbientStrenght = 0.1f;
    static_cast<Light*>(lightSrc01)->mAmbientColor = Vec3(0.9f, 0.9f, 0.9f);
    static_cast<Light*>(lightSrc01)->mLightStrenght = 0.5f;
    static_cast<Light*>(lightSrc01)->mLightColor = Vec3(0.9f, 0.9f, 0.9f);
    static_cast<Light*>(lightSrc01)->mSpecularStrenght = 0.3f;
    static_cast<Light*>(lightSrc01)->mSpecularExponent = 32;
    // Casting the shader to a PhongShader and setting the light by casting it from VisualObject to Light.
    static_cast<PhongShader*>(mShaderProgram[2])->setLight(static_cast<Light*>(lightSrc01));

    //Assigns an object to the interObj ptr.
    interObj = Ball;
    impactObj = Ball2;
    path = Curve;
    pathObj = ThePyramid;
    doorObj = Door;
    terrainObj = TerrainGraph;

    ScoreBoardPtr = ScoreBoard;

    gateObj = Gate;

    trophy01 = Box1;
    trophy02 = Box2;
    trophy03 = Box3;
    trophy04 = Box4;
    enemy = ThePyramid;

    // Interactive object
    if (interObj)
    {
        SceneObjects.push_back(interObj);
        SceneObjects2.push_back(interObj);
        SceneObjects3.push_back(interObj);
    }

    // =====================================================
    // ++++++++++++++++++++++|Scene 1|++++++++++++++++++++++++
    // =====================================================
    SceneObjects.push_back(lightSrc01);

    //SceneObjects.push_back(Ground);
    //
    //SceneObjects.push_back(WallLeft);
    //SceneObjects.push_back(WallRight);
    //SceneObjects.push_back(WallTop);
    //SceneObjects.push_back(WallBottom);
    //
    //SceneObjects.push_back(Tree1);
    //SceneObjects.push_back(Tree2);
    //SceneObjects.push_back(Tree3);
    //SceneObjects.push_back(Tree4);
    //SceneObjects.push_back(Tree5);
    //SceneObjects.push_back(Ball2);
    //SceneObjects.push_back(Gate);
    //
    //SceneObjects.push_back(Doughnut);

    SceneObjects.push_back(Terreng);


    // =====================================================
    // ++++++++++++++++++++++|Scene 2|++++++++++++++++++++++++
    // =====================================================
    SceneObjects2.push_back(lightSrc01);

    SceneObjects2.push_back(TerrainGraph);

    //SceneObjects2.push_back(WallLeft);
    //SceneObjects2.push_back(WallRight);
    //SceneObjects2.push_back(WallTop);
    //SceneObjects2.push_back(WallBottom);

    //SceneObjects2.push_back(TheOtherWall);
    //SceneObjects2.push_back(Building);
    //SceneObjects2.push_back(Door);
    //SceneObjects2.push_back(Flag);
    //SceneObjects2.push_back(Tree1);
    //SceneObjects2.push_back(Tree2);
    //SceneObjects2.push_back(Tree3);
    //SceneObjects2.push_back(Tree4);
    //SceneObjects2.push_back(Tree5);
    //SceneObjects2.push_back(IndoorsTetrahedron);

    // =====================================================
    // ++++++++++++++++++++++|Scene 3|++++++++++++++++++++++++
    // =====================================================
    SceneObjects3.push_back(lightSrc01);

    //SceneObjects3.push_back(WallLeft);
    //SceneObjects3.push_back(WallRight);
    //SceneObjects3.push_back(WallTop);
    //SceneObjects3.push_back(WallBottom);

    SceneObjects3.push_back(Box1);

    SceneObjects3.push_back(ThePyramid);

    //SceneObjects3.push_back(Ground);
    SceneObjects3.push_back(Box1);
    SceneObjects3.push_back(Box2);
    SceneObjects3.push_back(Box3);
    SceneObjects3.push_back(Box4);

    SceneObjects3.push_back(Gate);

    SceneObjects3.push_back(ScoreBoard);

    //Putter debugObjects inn i et array.
    for (auto it = DebugObjects.begin(); it != DebugObjects.end(); it++)
    {
        (*it)->init();
        (*it)->setShader(mShaderProgram[0]);
        (*it)->mMaterial.setTextureUnit(0);
        (*it)->mMaterial.mObjectColor = Vec3(1.f, 1.f, 1.f);
    }

    //Putter scene1 objektene inn i et array.
    for (auto it = SceneObjects.begin(); it != SceneObjects.end(); it++)
    {
        (*it)->setDynamicObject(interObj);
        (*it)->init();
        (*it)->setShader(mShaderProgram[2]);
        (*it)->mMaterial.setTextureUnit(0);
        (*it)->mMaterial.mObjectColor = Vec3(1.f, 0.f, 0.f);
    }

    //Putter scene2 objektene inn i et array.
    for (auto it = SceneObjects2.begin(); it != SceneObjects2.end(); it++)
    {
        (*it)->setDynamicObject(interObj);
        (*it)->init();
        (*it)->setShader(mShaderProgram[2]);
        (*it)->mMaterial.setTextureUnit(0);
        (*it)->mMaterial.mObjectColor = Vec3(0.f, 1.f, 0.f);
    }

    //Putter scene3 objektene inn i et array.
    for (auto it = SceneObjects3.begin(); it != SceneObjects3.end(); it++)
    {
        (*it)->setDynamicObject(interObj);
        (*it)->init();
        (*it)->setShader(mShaderProgram[2]);
        (*it)->mMaterial.setTextureUnit(0);
        (*it)->mMaterial.mObjectColor = Vec3(0.f, 0.f, 1.f);
    }

    //Setter egen farge på spiller ballen
    interObj->mMaterial.mObjectColor = Vec3(1.f, 1.f, 0.f);

    //Setter egne farger på bokser og fienden,
    Box1->mMaterial.mObjectColor = Vec3(0.f, 1.f, 1.f);
    Box2->mMaterial.mObjectColor = Vec3(0.f, 1.f, 1.f);
    Box3->mMaterial.mObjectColor = Vec3(0.f, 1.f, 1.f);
    Box4->mMaterial.mObjectColor = Vec3(0.f, 1.f, 1.f);

    ThePyramid->mMaterial.mObjectColor = Vec3(1.f, 0.f, 0.f);

    ////Setter egne farger på veggene
    //WallTop->mMaterial.mObjectColor = Vec3(0.9f, 0.f, 0.2f);
    //WallRight->mMaterial.mObjectColor = Vec3(0.9f, 0.f, 0.2f);
    //WallLeft->mMaterial.mObjectColor = Vec3(0.9f, 0.f, 0.2f);
    //WallBottom->mMaterial.mObjectColor = Vec3(0.9f, 0.f, 0.2f);
    //
    ////Setter egne verdier på doughnut
    //Doughnut->init();
    //Doughnut->setShader(mShaderProgram[1]);
    //Doughnut->mMaterial.setTextureUnit(4);
    //Doughnut->mMaterial.mObjectColor = Vec3(1.f, 1.f, 1.f);

    //Setter egne verdier på landskap
    TerrainGraph->init();
    TerrainGraph->setShader(mShaderProgram[2]);
    TerrainGraph->mMaterial.setTextureUnit(2);
    TerrainGraph->mMaterial.mObjectColor = Vec3(0.7f, 1.f, 0.1f);

    //Setter egne verdier på porten
    Gate->init();
    Gate->setShader(mShaderProgram[1]);
    Gate->mMaterial.setTextureUnit(1);
    Gate->mMaterial.mObjectColor = Vec3(1.f, 1.f, 1.f);

    //Setter egne verdier på billboard
    ScoreBoard->init();
    ScoreBoard->setShader(mShaderProgram[3]);
    ScoreBoard->mMaterial.setTextureUnit(3);
    ScoreBoard->mMaterial.mObjectColor = Vec3(1.f, 1.f, 1.f);

    static_cast<SpriteShader*>(mShaderProgram[3])->setSprite(static_cast<Billboard*>(ScoreBoard));

    //==============================================
    //+-+-+-+-+-+|Setting up camera locations & Systems|+-+-+-+-+-+
    //==============================================

    CamLoc01 = Vec3(0.f, 5.f, 12.f);
    CamLoc02 = Vec3(8.f, 2.f, -6.f);
    CamLoc03 = Vec3(0.f, 500.f, 250.f);
    CamLoc04 = Vec3(0.f, 0.f, 0.f);
    CamLoc05 = Vec3(0.f, 0.f, 0.f);

    Camera* tempCam = new Camera(Vec3(0.f, 3.f, -1.f));
    TheCamera = tempCam;

    // Sends the camera into each shader.
    mShaderProgram[0]->setCurrentCamera(TheCamera);
    mShaderProgram[1]->setCurrentCamera(TheCamera);
    mShaderProgram[2]->setCurrentCamera(TheCamera);
    mShaderProgram[3]->setCurrentCamera(TheCamera);

    glBindVertexArray( 0 );
}

///Called each frame - doing the rendering
void RenderWindow::render()
{
    processInputs();

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Since our shader uses a matrix and we rotate the triangle, we send the current matrix here
    //must be here to update each frame - if static object, it could be set only once

    //=======================================================================================================
    //------------------------------------------------------------------------------|Camera|-------------------------------------------------------------------------
    //=======================================================================================================

    TheCamera->perspective(fieldOfView, aspectRatio, 0.1f, -1.f);

    // CAMERA 1:
    if (!cameraInHouse && cameraMode == 0)
    {
        // What does the camera look at?
        const gs2019::Vector3D target (interObj->getPosition().x, interObj->getPosition().y, interObj->getPosition().z);

        TheCamera->position.x = interObj->getPosition().x;
        TheCamera->position.y = interObj->getPosition().y + 3.f;
        TheCamera->position.z = interObj->getPosition().z + 3.f;
        TheCamera->lookAt(target);
    }

    // CAMERA 2:
    else if (!cameraInHouse && cameraMode == 1)
    {
        // Where is the camera?
        TheCamera->position = CamLoc01;

        // What does the camera look at?
        const gs2019::Vector3D target (interObj->getPosition().x, interObj->getPosition().y, interObj->getPosition().z);
        TheCamera->lookAt(target);
    }
    // CAMERA 3:
    else if (cameraInHouse)
    {
        // Where is the camera?
        TheCamera->position = CamLoc02;

        // What does the camera look at?
        const gs2019::Vector3D target (interObj->getPosition().x, interObj->getPosition().y, interObj->getPosition().z);
        TheCamera->lookAt(target);
    }
    // CAMERA 4:
    else if (!cameraInHouse && cameraMode == 2)
    {
        // Where is the camera?
        TheCamera->position = CamLoc03;

        // What does the camera look at?
        const gs2019::Vector3D target (0.f, 0.f, -0.1f);
        TheCamera->lookAt(target);
    }


    //=======================================================================================================
    //---------------------------------------------------------------------------|Collision test|-----------------------------------------------------------------------
    //=======================================================================================================

    const gs2019::Vector3D distance = interObj->getPosition() - impactObj->getPosition();

    if (distance.length() < (interObj->getRadius() + impactObj->getRadius()))
    {
        bool objectMoveX = false;
        bool objectMoveY = false;

        if ((interObj->getPosition().x < impactObj->getPosition().x) && !objectMoveX)
        {
            impactObj->move(0.01f, 0.f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
            objectMoveX = true;
        }

        if ((interObj->getPosition().x > impactObj->getPosition().x) && !objectMoveX)
        {
            impactObj->move(-0.01f, 0.f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
            objectMoveX = true;
        }

        if ((interObj->getPosition().z < impactObj->getPosition().z) && !objectMoveY)
        {
            impactObj->move(0.f, 0.f, 0.01f, gs2019::Vector3D(0.f, 1.f, 0.f));
            objectMoveY = true;
        }

        if ((interObj->getPosition().z > impactObj->getPosition().z) && !objectMoveY)
        {
            impactObj->move(0.f, 0.f, -0.01f, gs2019::Vector3D(0.f, 1.f, 0.f));
            objectMoveY = true;
        }
    }

//    if ((interObj->getPosition().x > 3.f && interObj->getPosition().x < 9.f) && (interObj->getPosition().z > -7.f && interObj->getPosition().z < -1.f) && currentScene == 1)
//    {
//        cameraInHouse = true;
//    }
//    else
//    {
//        cameraInHouse = false;
//    }

    //================================================================================================================
    //------------------------------------------------------------------------------|ACTUAL DRAW CALL!!|-------------------------------------------------------------------------
    //================================================================================================================

    // Object moving along curve/spline
    // Updates time variable
    if (timeActive)
        time += 0.1f;

    float curveDistance = MathLab::abs(cos(time*0.1f));

    pathObj->setLocation(path->evaluateBezier(3, curveDistance).x, path->evaluateBezier(3, curveDistance).y, path->evaluateBezier(3, curveDistance).z);

    if (debug)
    {
        for (auto it = DebugObjects.begin(); it != DebugObjects.end(); it++)
        {
            (*it)->draw();
        }
    }

    // Scene 1
    if (currentScene == 0)
    {
        // Places the player on the surface of the terrain.
        interObj->setHeight(0.f);

        for (auto it = SceneObjects.begin(); it != SceneObjects.end(); it++)
        {
            (*it)->draw();
        }
    }

    // ===================== PHYSICS SCENE ===========================

    // Scene 2
    if (currentScene == 1)
    {
        // Sets the player y-axis value with barycentric coordinates
        Vec3 tempVector{interObj->getPosition().x, 0.f, interObj->getPosition().z};
        interObj->setHeight(terrainObj->findTargetHeight(tempVector) + interObj->radius);

        static_cast<OctahedronBall*>(interObj)->VertexA = terrainObj->vertexA;
        static_cast<OctahedronBall*>(interObj)->VertexB = terrainObj->vertexB;
        static_cast<OctahedronBall*>(interObj)->VertexC = terrainObj->vertexC;

        for (auto it = SceneObjects2.begin(); it != SceneObjects2.end(); it++)
        {
            (*it)->draw();
        }
    }

    // ============================================================

    // Scene 3
    if (currentScene == 2)
    {
        // Places the player on the surface of the terrain.
        interObj->setHeight(0.f);
        //interObj->setHeight(terrainObj->applyHeightFunction(interObj->getPosition().x, 0.2f, interObj->getPosition().z) + interObj->radius);

        for (auto it = SceneObjects3.begin(); it != SceneObjects3.end(); it++)
        {
            (*it)->draw();
        }
    }

    //+++++++++++++++++++++++
    //|||||-Game related checks-|||||
    //+++++++++++++++++++++++
    const gs2019::Vector3D trophyDistance01 = interObj->getPosition() - trophy01->getPosition();
    const gs2019::Vector3D trophyDistance02 = interObj->getPosition() - trophy02->getPosition();
    const gs2019::Vector3D trophyDistance03 = interObj->getPosition() - trophy03->getPosition();
    const gs2019::Vector3D trophyDistance04 = interObj->getPosition() - trophy04->getPosition();

    const gs2019::Vector3D enemyDistance = interObj->getPosition() - pathObj->getPosition();

    if (trophyDistance01.length() <= (0.5f + interObj->radius) && !trophy01Taken && (currentScene == 2))
    {
        trophy01->setLocation(0.f, -1000.f, 0.f);
        trophy01Taken = true;

        ++trophyCounter;
        static_cast<Billboard*>(ScoreBoardPtr)->updateUVFrame(trophyCounter);

        qDebug() << "TROPHY 1 PICKED UP!";
    }

    if (trophyDistance02.length() <= (0.5f + interObj->radius) && !trophy02Taken && (currentScene == 2))
    {
        trophy02->setLocation(0.f, -1000.f, 0.f);
        trophy02Taken = true;

        ++trophyCounter;
        static_cast<Billboard*>(ScoreBoardPtr)->updateUVFrame(trophyCounter);

        qDebug() << "TROPHY 2 PICKED UP!";
    }

    if (trophyDistance03.length() <= (0.5f + interObj->radius) && !trophy03Taken && (currentScene == 2))
    {
        trophy03->setLocation(0.f, -1000.f, 0.f);
        trophy03Taken = true;

        ++trophyCounter;
        static_cast<Billboard*>(ScoreBoardPtr)->updateUVFrame(trophyCounter);

        qDebug() << "TROPHY 3 PICKED UP!";
    }

    if (trophyDistance04.length() <= (0.5f + interObj->radius) && !trophy04Taken && (currentScene == 2))
    {
        trophy04->setLocation(0.f, -1000.f, 0.f);
        trophy04Taken = true;

        ++trophyCounter;
        static_cast<Billboard*>(ScoreBoardPtr)->updateUVFrame(trophyCounter);

        qDebug() << "TROPHY 4 PICKED UP!";
    }

    if (enemyDistance.length() <= (0.5f + interObj->radius) && !playerCollided && (currentScene == 2))
    {
        restartGame();
    }

    if (trophy01Taken && trophy02Taken && trophy03Taken && trophy04Taken)
    {
        gateObj->move(0.f, 0.1f, 0.1f, Vec3(0.f, 1.f ,0.f));

        static_cast<OctahedronBall*>(interObj)->gateOpen = true;

        if(interObj->getPosition().z < -10.f)
        {
            //qDebug() << "CONGRATULATIONS! YOU'VE WON THE GAME!!!!";
            static_cast<Billboard*>(ScoreBoardPtr)->updateUVFrame(5);
        }
        else
        {
            //qDebug() << "GO!!!!";
            static_cast<Billboard*>(ScoreBoardPtr)->updateUVFrame(4);
        }
    }
    //++++++ |GAME STUFF END| +++++++++++++++++++++++++++++++++++++

    //Calculate framerate before
    // checkForGLerrors() because that takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    //    checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);
}

//This function is called from Qt when window is exposed (shown)
//and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    // Sets the aspect ratio according to window size:
    float x = width();
    float y = height();
    aspectRatio =  x/y;

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact..)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

void RenderWindow::toggleDebugItems()
{
    debug = !debug;
}

void RenderWindow::toggleWireframe()
{
    displayWireframe = !displayWireframe;

    if(displayWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void RenderWindow::smoothTerrain()
{
    static_cast<graph*>(terrainObj)->makeSmooth();
    terrainObj->init();
}

void RenderWindow::startGame()
{
    currentScene = 2;

    cameraMode = 1;

    static_cast<OctahedronBall*>(interObj)->stopVelocity();
    interObj->setLocation(0.f, 0.f, 0.f);
}

void RenderWindow::pauseGame()
{
    timeActive = !timeActive;
    static_cast<OctahedronBall*>(interObj)->stopVelocity();
}

void RenderWindow::restartGame()
{
    playerCollided = true;
    interObj->setLocation(playerStartPos.x, playerStartPos.y, playerStartPos.z);

    qDebug() << "GAME OVER!";

    trophyCounter = 0;
    static_cast<Billboard*>(ScoreBoardPtr)->updateUVFrame(0);
    static_cast<OctahedronBall*>(interObj)->stopVelocity();
    static_cast<OctahedronBall*>(interObj)->resetPhysics();
    //static_cast<OctahedronBall*>(interObj)->gateOpen = false;

    trophy01Taken = false;
    trophy02Taken = false;
    trophy03Taken = false;
    trophy04Taken = false;

    trophy01->setLocation(trophyLoc01.x, trophyLoc01.y, trophyLoc01.z);
    trophy02->setLocation(trophyLoc02.x, trophyLoc02.y, trophyLoc02.z);
    trophy03->setLocation(trophyLoc03.x, trophyLoc03.y, trophyLoc03.z);
    trophy04->setLocation(trophyLoc04.x, trophyLoc04.y, trophyLoc04.z);

    Vec3 gateLoc(9.f, 0.5f, -10.f);
    gateObj->setLocation(gateLoc.x, gateLoc.y, gateLoc.z);

    playerCollided = false;
}

void RenderWindow::resetPlayer()
{
    static_cast<OctahedronBall*>(interObj)->stopVelocity();
    interObj->setLocation(0.f, 0.f, 0.f);
}

//The way this is set up is that we start the clock before doing the draw call,
//and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)    //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30) //once pr 30 frames = update the message twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

/// Uses QOpenGLDebugLogger if this is present
/// Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

/// Tries to start the extended OpenGL debugger that comes with Qt
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            qDebug() << "This system can not use QOpenGLDebugLogger, so we revert to glGetError()";

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
        }
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    pressedKeys += event->key();

    float jumpForce = 2.f;

    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        mMainWindow->close();
    }

    if (event->key() == Qt::Key_Q)
    {
        ++currentScene;

        if(currentScene > 2)
        {
            currentScene = 0;
        }
    }

    if (event->key() == Qt::Key_R)
    {
        ++cameraMode;

        if(cameraMode > 2)
        {
            cameraMode = 0;
        }
    }

    if (event->key() == Qt::Key_T)
    {
        toggleWireframe();
    }

    if (event->key() == Qt::Key_Space)
    {
        interObj->move(0.f, jumpForce, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    pressedKeys -= event->key();
}

void RenderWindow::processInputs()
{
    float moveSpeed = 0.005f;
    float rotationSpeed = 5.f;

    for (auto i = pressedKeys.begin(); i != pressedKeys.end(); i++)
    {
        if (timeActive)
        {
            if (*i == Qt::Key_D)
            {
                interObj->move(moveSpeed, 0.f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
            }

            if (*i == Qt::Key_A)
            {
                interObj->move(-moveSpeed, 0.f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
            }

            if (*i == Qt::Key_W)
            {
                interObj->move(0.f, 0.f, -moveSpeed, gs2019::Vector3D(0.f, 1.f, 0.f));
            }

            if (*i == Qt::Key_S)
            {
                interObj->move(0.f, 0.f, moveSpeed, gs2019::Vector3D(0.f, 1.f, 0.f));
            }

            if (*i == Qt::Key_Left)
            {
                doorObj->rotate(rotationSpeed, 0, 1, 0);
            }

            if (*i == Qt::Key_Right)
            {
                doorObj->rotate(-rotationSpeed, 0, 1, 0);
            }
        }
    }
}
