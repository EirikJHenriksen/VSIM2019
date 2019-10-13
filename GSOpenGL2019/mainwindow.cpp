#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDebug>

#include "renderwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

void MainWindow::init()
{
    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    //QWidget::showMaximized();
    resize(QDesktopWidget().availableGeometry(this).size() * 0.8);

    //OpenGL v 4.1 - (Ole Flatens Mac does not support higher than this...)
    //you can try other versions, but then have to update RenderWindow and Shader
    //to inherit from other than QOpenGLFunctions_4_1_Core
    format.setVersion(4, 1);
    //Using the main profile for OpenGL - no legacy code permitted
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types that OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);
    //Set the number of samples used for multisampling
    format.setSamples(8);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The renderer will need a depth buffer - (not requiered to set in glfw-tutorials)
    format.setDepthBufferSize(24);

    //Just prints out what OpenGL format we will get
    // - this can be deleted
    qDebug() << "Requesting surface format: " << format;

    //We have a format for the OpenGL window, so let's make it:
    mRenderWindow = new RenderWindow(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderWindow->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    //OpenGLLayout is made in the .ui-file!
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();
}

//Example of a slot called from the button on the top of the program.
void MainWindow::on_pushButton_clicked()
{
    qDebug() << "The suspicious button does nothing! Or does it?";
}

void MainWindow::on_actionControls_triggered()
{
    QMessageBox::information(this, "Controls", "Control the ball using the W, A, S, D - keys.\n\nHotkeys:\nQ: Swap between levels.\nR: Change Camera Angle.\nT: Toggle Wireframe.");
}

void MainWindow::on_actionToggle_Wireframe_triggered()
{
    mRenderWindow->toggleWireframe();
}

void MainWindow::on_actionAxis_triggered()
{
    mRenderWindow->toggleDebugItems();
}

void MainWindow::on_actionSmooth_Terrain_triggered()
{
    mRenderWindow->smoothTerrain();
}

void MainWindow::on_actionLevel_1_triggered()
{
    mRenderWindow->resetPlayer();
    mRenderWindow->currentScene = 0;
}

void MainWindow::on_actionLevel_2_triggered()
{
    mRenderWindow->resetPlayer();
    mRenderWindow->currentScene = 1;
}

void MainWindow::on_actionStart_triggered()
{
    mRenderWindow->startGame();
}

void MainWindow::on_actionStop_triggered()
{
    mRenderWindow->pauseGame();
}

void MainWindow::on_actionRestart_triggered()
{
    mRenderWindow->restartGame();
}

void MainWindow::on_actionReset_Player_triggered()
{
    mRenderWindow->resetPlayer();
}

void MainWindow::on_actionView_1_triggered()
{
    mRenderWindow->cameraMode = 1;
}

void MainWindow::on_actionView_2_triggered()
{
    mRenderWindow->cameraMode = 2;
}

void MainWindow::on_actionView_3_triggered()
{
    mRenderWindow->cameraMode = 0;
}
