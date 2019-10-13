#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_actionControls_triggered();

    void on_actionToggle_Wireframe_triggered();

    void on_actionAxis_triggered();

    void on_actionSmooth_Terrain_triggered();

    void on_actionLevel_1_triggered();

    void on_actionLevel_2_triggered();

    void on_actionStart_triggered();

    void on_actionStop_triggered();

    void on_actionRestart_triggered();

    void on_actionReset_Player_triggered();

    void on_actionView_1_triggered();

    void on_actionView_2_triggered();

    void on_actionView_3_triggered();

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
};

#endif // MAINWINDOW_H
