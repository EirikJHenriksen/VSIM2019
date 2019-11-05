#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include "Npc.h"

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
    mTexture[5] = new Texture("../GSOpenGL2019/Data/Textures/HeliSpiller.png", 5);
    mTexture[6] = new Texture("../GSOpenGL2019/Data/Textures/HeliFiende.png", 6);

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

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, mTexture[5]->id());

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, mTexture[6]->id());

    // =====================================================
    // ++++++++++++++++++++|Makes objects|++++++++++++++++++++++
    // =====================================================

    // ================ TERRAIN GRAPH ================
    graph* TerrainGraph = new graph(true);
    // =======================================================

    //BezierCurve* Curve = new BezierCurve(trophyLoc03, trophyLoc01, trophyLoc02, trophyLoc04, 3, 100, true, true);
    VisualObject* ThePyramid = new TriangleSurface("Moving_Pyramid", "../GSOpenGL2019/Data/Models/Pyramid.nei", Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false, false);
    //DebugObjects.push_back(Curve);

    //VisualObject* IndoorsTetrahedron= new TriangleSurface("Tetrahedron", "../GSOpenGL2019/Data/Models/Tetrahedron.nei", Vec3(6.f, 0.f, -4.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(1.f, 1.f, 1.f), true, true, false);

    // ================ PLAYER BALL/PHYSICS BALL ===================
    //VisualObject* Ball = new OctahedronBall("Player", 4, playerStartPos, Vec3(0.f, 0.f, 0.f), 0.f, Vec3(0.5f, 0.5f, 0.5f), false, false);
    // =======================================================

    //VisualObject* Ball2 = new OctahedronBall("Small_Ball", 4, Vec3(3.f, -0.3f, 0.f), Vec3(0.f, 0.f, 0.f), 0.f, Vec3(0.2f, 0.2f, 0.2f), false, false);

    // OBJ-Objects
    //VisualObject* Doughnut = new ObjMesh("Doughnut", "../GSOpenGL2019/Data/Models/Doughnut.obj", Vec3(3.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 10.f, Vec3(1.f, 1.f, 1.f), true, true);

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // [VSIM TING!!!]
    VisualObject* Spiller = new ObjMesh("SpillerHelikopter", "../GSOpenGL2019/Data/Models/Helikopter.obj", Vec3(-3.f, 0.f, -20.f), Vec3(0.f, 0.f, 1.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false);
    VisualObject* Fiende = new ObjMesh("FiendeHelikopter", "../GSOpenGL2019/Data/Models/Helikopter.obj", Vec3(3.f, 0.f, 10.f), Vec3(0.f, 0.f, 1.f), 0.f, Vec3(1.f, 1.f, 1.f), false, false);

    VisualObject* Stjerne01 = new ObjMesh("Sjerne01", "../GSOpenGL2019/Data/Models/Star.obj", Vec3(-40.f, 0.f, -40.f), Vec3(0.f, 0.f, 1.f), 0.f, Vec3(1.f, 1.f, 1.f), true, false);
    VisualObject* Stjerne02 = new ObjMesh("Sjerne01", "../GSOpenGL2019/Data/Models/Star.obj", Vec3(-20.f, 0.f, 40.f), Vec3(0.f, 0.f, 1.f), 0.f, Vec3(1.f, 1.f, 1.f), true, false);
    VisualObject* Stjerne03 = new ObjMesh("Sjerne01", "../GSOpenGL2019/Data/Models/Star.obj", Vec3(20.f, 0.f, -40.f), Vec3(0.f, 0.f, 1.f), 0.f, Vec3(1.f, 1.f, 1.f), true, false);
    VisualObject* Stjerne04 = new ObjMesh("Sjerne01", "../GSOpenGL2019/Data/Models/Star.obj", Vec3(40.f, 0.f, 40.f), Vec3(0.f, 0.f, 1.f), 0.f, Vec3(1.f, 1.f, 1.f), true, false);

    mSpiller = Spiller;
    mFiende = Fiende;

    mStjerne01 = Stjerne01;
    mStjerne02 = Stjerne02;
    mStjerne03 = Stjerne03;
    mStjerne04 = Stjerne04;

    mStartpunkt = Vec3(0.f, 0.f, 0.f);
    mSluttpunkt = Vec3(10.f, 10.f, 10.f);

    SceneObjects2.push_back(mSpiller);
    SceneObjects2.push_back(mFiende);
    SceneObjects2.push_back(mStjerne01);
    SceneObjects2.push_back(mStjerne02);
    SceneObjects2.push_back(mStjerne03);
    SceneObjects2.push_back(mStjerne04);

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
    //interObj = Ball;
    //impactObj = Ball2;
    //path = Curve;
    //doorObj = Door;
    terrainObj = TerrainGraph;

    // Interactive object
//    if (interObj)
//    {
//        SceneObjects.push_back(interObj);
//        SceneObjects2.push_back(interObj);
//        SceneObjects3.push_back(interObj);
//    }

    // ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // Oblig 3 stuff
    std::vector<Vec3> ctrlPoints;

    splineKnots.push_back(0.f);
    splineKnots.push_back(0.f);
    splineKnots.push_back(0.f);
    splineKnots.push_back(0.f);

    splineKnots.push_back(1.f);
    splineKnots.push_back(1.f);
    splineKnots.push_back(1.f);
    splineKnots.push_back(1.f);

    ctrlPoints.push_back(Vec3(-40.f, 0.f, -40.f));
    ctrlPoints.push_back(Vec3(-20.f, 0.f, 40.f));
    ctrlPoints.push_back(Vec3(20.f, 0.f, -40.f));
    ctrlPoints.push_back(Vec3(40.f, 0.f, 40.f));

    VisualObject* Curve = new BSplineCurve(splineKnots, ctrlPoints, 3);
    TheBSpline = static_cast<BSplineCurve*>(Curve);

    DebugObjects.push_back(static_cast<VisualObject*>(TheBSpline));

    FiendeStatemachine = new Npc(TheBSpline, Fiende);

    // =====================================================
    // ++++++++++++++++++++++|Scene 1|++++++++++++++++++++++++
    // =====================================================
    SceneObjects.push_back(lightSrc01);

    // =====================================================
    // ++++++++++++++++++++++|Scene 2|++++++++++++++++++++++++
    // =====================================================
    SceneObjects2.push_back(lightSrc01);

    SceneObjects2.push_back(TerrainGraph);


    // =====================================================
    // ++++++++++++++++++++++|Scene 3|++++++++++++++++++++++++
    // =====================================================
    SceneObjects3.push_back(lightSrc01);

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
    //interObj->mMaterial.mObjectColor = Vec3(1.f, 1.f, 0.f);

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

    // /////////////////////////////////////////////////////////////
    // SETTER EGNE VERDIER PÅ DE VIKTIGE OBJEKTENE!!!
    // Spiller
    static_cast<ObjMesh*>(Spiller)->init();
    static_cast<ObjMesh*>(Spiller)->setShader(mShaderProgram[1]);
    static_cast<ObjMesh*>(Spiller)->mMaterial.setTextureUnit(5);
    static_cast<ObjMesh*>(Spiller)->mMaterial.mObjectColor = Vec3(1.f, 1.f, 1.f);
    // Fiende
    static_cast<ObjMesh*>(Fiende)->init();
    static_cast<ObjMesh*>(Fiende)->setShader(mShaderProgram[1]);
    static_cast<ObjMesh*>(Fiende)->mMaterial.setTextureUnit(6);
    static_cast<ObjMesh*>(Fiende)->mMaterial.mObjectColor = Vec3(1.f, 1.f, 1.f);
    // Stjerne
    static_cast<ObjMesh*>(Stjerne01)->init();
    static_cast<ObjMesh*>(Stjerne01)->setShader(mShaderProgram[2]);
    static_cast<ObjMesh*>(Stjerne01)->mMaterial.setTextureUnit(0);
    static_cast<ObjMesh*>(Stjerne01)->mMaterial.mObjectColor = Vec3(1.f, 0.9f, 0.5f);
    static_cast<ObjMesh*>(Stjerne02)->init();
    static_cast<ObjMesh*>(Stjerne02)->setShader(mShaderProgram[2]);
    static_cast<ObjMesh*>(Stjerne02)->mMaterial.setTextureUnit(0);
    static_cast<ObjMesh*>(Stjerne02)->mMaterial.mObjectColor = Vec3(1.f, 0.9f, 0.5f);
    static_cast<ObjMesh*>(Stjerne03)->init();
    static_cast<ObjMesh*>(Stjerne03)->setShader(mShaderProgram[2]);
    static_cast<ObjMesh*>(Stjerne03)->mMaterial.setTextureUnit(0);
    static_cast<ObjMesh*>(Stjerne03)->mMaterial.mObjectColor = Vec3(1.f, 0.9f, 0.5f);
    static_cast<ObjMesh*>(Stjerne04)->init();
    static_cast<ObjMesh*>(Stjerne04)->setShader(mShaderProgram[2]);
    static_cast<ObjMesh*>(Stjerne04)->mMaterial.setTextureUnit(0);
    static_cast<ObjMesh*>(Stjerne04)->mMaterial.mObjectColor = Vec3(1.f, 0.9f, 0.5f);

    //==============================================
    //+-+-+-+-+-+|Setting up camera locations & Systems|+-+-+-+-+-+
    //==============================================

    CamLoc01 = Vec3(0.f, 5.f, 12.f);
    CamLoc02 = Vec3(8.f, 2.f, -6.f);
    CamLoc03 = Vec3(0.f, 20.f, 10.f); //Vec3(0.f, 500.f, 250.f);
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

    FiendeStatemachine->update();

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
        //const gs2019::Vector3D target (interObj->getPosition().x, interObj->getPosition().y, interObj->getPosition().z);
        const gs2019::Vector3D target (mSpiller->getPosition().x, mSpiller->getPosition().y, mSpiller->getPosition().z);

        TheCamera->position.x = mSpiller->getPosition().x;
        TheCamera->position.y = mSpiller->getPosition().y + 5.f;
        TheCamera->position.z = mSpiller->getPosition().z - 25.f;
        TheCamera->lookAt(target);
    }

    // CAMERA 2:
    else if (!cameraInHouse && cameraMode == 1)
    {
        // Where is the camera?
        TheCamera->position = CamLoc01;

        // What does the camera look at?
        //const gs2019::Vector3D target (interObj->getPosition().x, interObj->getPosition().y, interObj->getPosition().z);
        const gs2019::Vector3D target (mSpiller->getPosition().x, mSpiller->getPosition().y, mSpiller->getPosition().z);
        TheCamera->lookAt(target);
    }
    // CAMERA 3:
    else if (cameraInHouse)
    {
        // Where is the camera?
        TheCamera->position = CamLoc02;

        // What does the camera look at?
        //const gs2019::Vector3D target (interObj->getPosition().x, interObj->getPosition().y, interObj->getPosition().z);
        const gs2019::Vector3D target (mSpiller->getPosition().x, mSpiller->getPosition().y, mSpiller->getPosition().z);
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

//    const gs2019::Vector3D distance = interObj->getPosition() - impactObj->getPosition();

//    if (distance.length() < (interObj->getRadius() + impactObj->getRadius()))
//    {
//        bool objectMoveX = false;
//        bool objectMoveY = false;

//        if ((interObj->getPosition().x < impactObj->getPosition().x) && !objectMoveX)
//        {
//            impactObj->move(0.01f, 0.f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
//            objectMoveX = true;
//        }

//        if ((interObj->getPosition().x > impactObj->getPosition().x) && !objectMoveX)
//        {
//            impactObj->move(-0.01f, 0.f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
//            objectMoveX = true;
//        }

//        if ((interObj->getPosition().z < impactObj->getPosition().z) && !objectMoveY)
//        {
//            impactObj->move(0.f, 0.f, 0.01f, gs2019::Vector3D(0.f, 1.f, 0.f));
//            objectMoveY = true;
//        }

//        if ((interObj->getPosition().z > impactObj->getPosition().z) && !objectMoveY)
//        {
//            impactObj->move(0.f, 0.f, -0.01f, gs2019::Vector3D(0.f, 1.f, 0.f));
//            objectMoveY = true;
//        }
//    }

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

    // OBLIG 3 -- Horrible game logic
    if (time > 1.f)
    {
        //float curveTime = MathLab::abs(cos(time*0.02f));
        //mFiende->setLocation(TheBSpline->travelAlongSpline(curveTime).x, TheBSpline->travelAlongSpline(curveTime).y, TheBSpline->travelAlongSpline(curveTime).z);

        if ((mStjerne01->getPosition() - mSpiller->getPosition()).length() < 1.f)
        {
            mStjerne01->setLocation(0.f, -1000.f, 0.f);
            mStjerne01Taken = true;
            ctrlPointsChanged = true;
        }

        if ((mStjerne02->getPosition() - mSpiller->getPosition()).length() < 1.f)
        {
            mStjerne02->setLocation(0.f, -1000.f, 0.f);
            mStjerne02Taken = true;
            ctrlPointsChanged = true;
        }

        if ((mStjerne03->getPosition() - mSpiller->getPosition()).length() < 1.f)
        {
            mStjerne03->setLocation(0.f, -1000.f, 0.f);
            mStjerne03Taken = true;
            ctrlPointsChanged = true;
        }

        if ((mStjerne04->getPosition() - mSpiller->getPosition()).length() < 1.f)
        {
            mStjerne04->setLocation(0.f, -1000.f, 0.f);
            mStjerne04Taken = true;
            ctrlPointsChanged = true;
        }

        std::vector<Vec3> ctrlPoints;

        if (ctrlPointsChanged)
        {
            // Lastes inn uansett! (Startpunkt)
            ctrlPoints.push_back(Vec3(-40.f, 0.f, -40.f));

            if (!mStjerne02Taken)
            {
                ctrlPoints.push_back(Vec3(-20.f, 0.f, 40.f));
            }

            if (!mStjerne02Taken)
            {
                ctrlPoints.push_back(Vec3(20.f, 0.f, -40.f));
            }

            // Lastes inn uansett! (Sluttpunkt)
            ctrlPoints.push_back(Vec3(40.f, 0.f, 40.f));

            TheBSpline->setKnotsAndControlPoints(splineKnots, ctrlPoints);

            if (mStjerne01Taken && mStjerne02Taken && mStjerne03Taken && mStjerne04Taken)
            {
                mFiendeDead = true;
            }
        }

        if ((mFiende->getPosition() - mSpiller->getPosition()).length() < 1.f || mSpillerDead)
        {
            static_cast<VisualObject*>(mSpiller)->mContRot = true;
            mSpiller->move(0.f, -0.1f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
            mSpillerDead = true;
        }

        if (mFiendeDead)
        {
            static_cast<VisualObject*>(mFiende)->mContRot = true;
            mFiende->move(0.f, -0.1f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
        }
    }


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
        //Vec3 tempVector{mSpiller->getPosition().x, 0.f, mSpiller->getPosition().z};
        //mSpiller->setHeight(terrainObj->findTargetHeight(tempVector) + 0.f);

        //mSpiller->setHeight(-10.f);

        //static_cast<OctahedronBall*>(interObj)->VertexA = terrainObj->vertexA;
        //static_cast<OctahedronBall*>(interObj)->VertexB = terrainObj->vertexB;
        //static_cast<OctahedronBall*>(interObj)->VertexC = terrainObj->vertexC;

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

void RenderWindow::resetPlayer()
{
    static_cast<OctahedronBall*>(interObj)->stopVelocity();
    //interObj->setLocation(0.f, 0.f, 0.f);
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
    float moveSpeed = 0.15f;
    float rotationSpeed = 5.f;

    for (auto i = pressedKeys.begin(); i != pressedKeys.end(); i++)
    {
        if (timeActive)
        {
            if (*i == Qt::Key_D)
            {
                //interObj->move(moveSpeed, 0.f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
                //mSpiller->move(-moveSpeed, 0.f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
                mSpiller->mMatrix.rotate(-moveSpeed*10.f, 0.f, 1.f, 0.f);
            }

            if (*i == Qt::Key_A)
            {
                //interObj->move(-moveSpeed, 0.f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
                //mSpiller->move(moveSpeed, 0.f, 0.f, gs2019::Vector3D(0.f, 1.f, 0.f));
                mSpiller->mMatrix.rotate(moveSpeed*10.f, 0.f, 1.f, 0.f);
            }

            if (*i == Qt::Key_W)
            {
                //interObj->move(0.f, 0.f, -moveSpeed, gs2019::Vector3D(0.f, 1.f, 0.f));
                mSpiller->move(0.f, 0.f, moveSpeed, gs2019::Vector3D(0.f, 1.f, 0.f));
            }

            if (*i == Qt::Key_S)
            {
                //interObj->move(0.f, 0.f, moveSpeed, gs2019::Vector3D(0.f, 1.f, 0.f));
                mSpiller->move(0.f, 0.f, -moveSpeed, gs2019::Vector3D(0.f, 1.f, 0.f));
            }
        }
    }
}
