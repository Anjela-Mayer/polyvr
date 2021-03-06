#include <OpenSG/OSGGLUT.h>

#include "VRGlutWindow.h"

#include "../devices/VRMouse.h"
#include "../devices/VRKeyboard.h"

OSG_BEGIN_NAMESPACE;
using namespace std;

map<int, VRGlutWindow*> glutWindows;

VRGlutWindow* getCurrentWindow() {
#ifndef WASM
    return glutWindows[glutGetWindow()];
#else
    return glutWindows.begin()->second;
#endif
}

void glutResize(int w, int h) { getCurrentWindow()->resize(w, h); }
void glutMouse(int b, int s, int x, int y) { getCurrentWindow()->onMouse(b ,s ,x ,y); }
void glutMotion(int x, int y) { getCurrentWindow()->onMotion(x, y); }
void glutKeyboard(unsigned char k, int x, int y) { getCurrentWindow()->onKeyboard(k, 1, x, y); }
void glutSpecial(int k, int x, int y) { getCurrentWindow()->onKeyboard_special(k, 1, x, y); }
void glutKeyboardUp(unsigned char k, int x, int y) { getCurrentWindow()->onKeyboard(k, 0, x, y); }
void glutSpecialUp(int k, int x, int y) { getCurrentWindow()->onKeyboard_special(k, 0, x, y); }

VRGlutWindow::VRGlutWindow() {
    cout << "Glut: New Window" << endl;
    type = 1;

    int width = 20;
    int height = 10;

    glutInitWindowSize(width, height);
    winID = glutCreateWindow("PolyVR");

    GLUTWindowMTRecPtr win = GLUTWindow::create();
    _win = win;
    win->setGlutId(winID);
    win->setSize(width, height);
    win->init();

    glutWindows[winID] = this;

    glutReshapeFunc(glutResize);
    glutKeyboardFunc(glutKeyboard);
    glutSpecialFunc(glutSpecial);
    glutKeyboardUpFunc(glutKeyboardUp);
    glutSpecialUpFunc(glutSpecialUp);
    glutMotionFunc(glutMotion);
    glutMouseFunc(glutMouse);
}

VRGlutWindow::~VRGlutWindow() {
    glutDestroyWindow(winID);
    win = NULL;
}

VRGlutWindowPtr VRGlutWindow::ptr() { return static_pointer_cast<VRGlutWindow>( shared_from_this() ); }
VRGlutWindowPtr VRGlutWindow::create() { return VRGlutWindowPtr(new VRGlutWindow() ); }

void VRGlutWindow::save(XMLElementPtr node) { VRWindow::save(node); }
void VRGlutWindow::load(XMLElementPtr node) { VRWindow::load(node); }

void VRGlutWindow::onMouse(int b, int s, int x, int y) {
    if (auto m = getMouse()) m->mouse(b, s, x, y);
}

void VRGlutWindow::onMotion(int x, int y) {
    if (auto m = getMouse()) m->motion(x, y);
}

void VRGlutWindow::onKeyboard(int c, int s, int x, int y) {
    if (auto k = getKeyboard()) k->keyboard(c, s, x, y);
}

void VRGlutWindow::onKeyboard_special(int c, int s, int x, int y) {
    if (auto k = getKeyboard()) k->keyboard_special(c, s, x, y);
}

OSG_END_NAMESPACE;




