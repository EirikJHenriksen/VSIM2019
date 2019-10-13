#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include "beziercurve.h"

int main(int argc, char *argv[])
{
    //================| TEST |================

    // Tester om funksjonene virker.
    //gs2019::Vector3D test1(1,0,-2);
    //gs2019::Vector3D test2(2,-3,2);
    //float result;

    //test1 = test1^test2;
    //qDebug() << test1.x << " " << test1.y << " " << test1.z;

    //result = test1*test2;
    //qDebug() << result;

    //test1 = test1+test2;
    //qDebug() << test1.x << " " << test1.y << " " << test1.z;

    //test1 = test1*5;
    //qDebug() << test1.x << " " << test1.y << " " << test1.z;

    // 1337-Matrix!
    //gs2019::Vector4D test1(2.f, 5.f, 7.f, 2.f);
    //gs2019::Vector4D test2(7.f, 9.f, 5.f, 1.f);
    //gs2019::Vector4D test3(1.f, 9.f, 4.f, 4.f);
    //gs2019::Vector4D test4(2.f, 3.f, 2.f, 6.f);

    //gs2019::Vector4D test1(1.f, 7.f, 3.f, 4.f);
    //gs2019::Vector4D test2(1.f, 1.f, 2.f, 1.f);
    //gs2019::Vector4D test3(3.f, 1.f, 9.f, 4.f);
    //gs2019::Vector4D test4(3.f, 3.f, 1.f, 6.f);

    //gs2019::Matrix4x4 TheMatrix(test1, test2, test3, test4);
    //TheMatrix.debugPrint();
    //TheMatrix * 2;
    //TheMatrix.debugPrint();

    //TheMatrix.LU();
    //gs2019::Vector4D nope = TheMatrix*test1;
    //nope.debugPrint();
    //================| TEST END |================

    //gs2019::Vector4D testA(1.f, 2.f, 3.f, 4.f);
    //TheMatrix.solve(testA);
    //qDebug() << testA.x << "  " << testA.y << "   " << testA.z << "   " << testA.w;

    //Forces the usage of desktop OpenGL
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes an Qt application
    QApplication a(argc, argv);

    //Makes the Qt MainWindow and shows it.
    MainWindow w;
    w.show();

    return a.exec();
}
