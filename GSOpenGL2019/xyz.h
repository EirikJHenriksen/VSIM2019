#ifndef XYZ_H
#define XYZ_H

#include "visualobject.h"

class XYZ : public VisualObject
{
private:

public:
    XYZ();
    ~XYZ() override;

    void init() override;
    void draw() override;
};


#endif // XYZ_H
