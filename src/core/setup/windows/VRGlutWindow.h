#ifndef VRGLUTWINDOW_H_INCLUDED
#define VRGLUTWINDOW_H_INCLUDED

#include "VRWindow.h"
#include <OpenSG/OSGGLUTWindow.h>

OSG_BEGIN_NAMESPACE;
using namespace std;


class VRGlutWindow : public VRWindow {
    private:
        GLUTWindowMTRecPtr win;
        int winID;

    public:
        VRGlutWindow();
        ~VRGlutWindow();

        static VRGlutWindowPtr create();
        VRGlutWindowPtr ptr();

        void save(XMLElementPtr node);
        void load(XMLElementPtr node);
};

OSG_END_NAMESPACE;

#endif // VRGLUTWINDOW_H_INCLUDED
