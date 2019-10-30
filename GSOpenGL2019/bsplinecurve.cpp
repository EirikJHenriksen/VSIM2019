#include "bsplinecurve.h"

BSplineCurve::BSplineCurve()
{

}

void BSplineCurve::setKnotsAndControlPoints(std::vector<float> knots, std::vector<Vec3> points)
{
    for (int i = 0; i < 4; i++)
    {
        b[i] = points[i];
    }
}

// Parametre inn:
// x - en parameterverdi på skjøtvektor
// my - et tall slik at t[my] <= x < t[my+1]
// Returverdi: et punkt på splinekurven
// b,n,d,t er objektvariable i klassen BSplineCurve
Vec3 BSplineCurve::evaluateBSpline(int my, float x)
{
    Vec3 a[20]; // forutsetter da at n+d+1 <= 20
    for (int j=0; j<=d; j++)
        a[d-j] = b[my-j];

    for (int k=d; k>0; k--) {
        int j = my-k;
        for (int i=0; i<k; i++) {
            j++;
            float w = (x-t[j])/(t[j+k]-t[j]);
            a[i] = a[i] * (1-w) + a[i+1] * w;
        }
    }
    return a[0];
}

