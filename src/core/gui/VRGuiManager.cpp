#include "VRGuiManager.h"
#include "core/scene/VRSceneManager.h"
#include "VRGuiUtils.h"
#include "VRDemos.h"
#include "VRGuiScene.h"
#include "VRGuiBits.h"
#include "VRGuiNav.h"
#include "VRGuiSignals.h"
#include "VRGuiScripts.h"
#include "VRGuiSetup.h"
#include "VRGuiNet.h"
#include "VRGuiGeneral.h"
#include <gtkmm/uimanager.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtk/gtkglinit.h>

OSG_BEGIN_NAMESPACE;
using namespace std;

VRGuiScene* g_scene;
VRGuiBits* g_bits;
VRDemos* g_demos;
VRGuiNav* g_nav;
VRGuiScripts* g_sc;
VRGuiSetup* g_di;
VRGuiNet* g_net;
VRGuiGeneral* g_gen;
Gtk::Main* GtkMain;

VRGuiManager::VRGuiManager() {
    int argc   = 0;
    GtkMain = new Gtk::Main(&argc, NULL, false);
    gtk_gl_init(&argc, NULL);

    //gtk_rc_parse("gui/gtkrc");
    g_demos = new VRDemos();
    g_scene = new VRGuiScene();
    g_bits = new VRGuiBits();
    g_nav = new VRGuiNav();
    g_sc = new VRGuiScripts();
    g_di = new VRGuiSetup();
    g_net = new VRGuiNet();
    g_gen = new VRGuiGeneral();
    g_scene->updateTreeView();

    VRDevCb* fkt;
    fkt = new VRDevCb("GUI_updateSceneViewer", boost::bind(&VRGuiScene::updateTreeView, g_scene) );
    VRGuiSignals::get()->getSignal("scene_modified")->add( fkt );
    VRGuiSignals::get()->getSignal("scene_changed")->add( fkt );
    VRGuiSignals::get()->getSignal("camera_changed")->add(fkt);

    fkt = new VRDevCb("GUI_updateBits", boost::bind(&VRGuiBits::update, g_bits) );
    VRGuiSignals::get()->getSignal("scene_changed")->add( fkt );
    VRGuiSignals::get()->getSignal("camera_added")->add( fkt );

    fkt = new VRDevCb("GUI_updateNav", boost::bind(&VRGuiNav::update, g_nav) );
    VRGuiSignals::get()->getSignal("scene_changed")->add( fkt );

    fkt = new VRDevCb("GUI_updateScripts", boost::bind(&VRGuiScripts::updateList, g_sc) );
    VRGuiSignals::get()->getSignal("scene_changed")->add( fkt );

    fkt = new VRDevCb("GUI_updateProtocols", boost::bind(&VRGuiNet::updateList, g_net) );
    VRGuiSignals::get()->getSignal("scene_changed")->add( fkt );

    fkt = new VRDevCb("GUI_updateBackground", boost::bind(&VRGuiGeneral::updateScene, g_gen) );
    VRGuiSignals::get()->getSignal("scene_changed")->add( fkt );

    VRFunction<int>* ufkt = new VRFunction<int>("GUI_updateManager", boost::bind(&VRGuiManager::update, this) );
    VRSceneManager::get()->addUpdateFkt(ufkt, 1);

    Gtk::Window* top = 0;
    VRGuiBuilder()->get_widget("window1", top);
    top->maximize();
    top->show_all();
}

VRGuiManager::~VRGuiManager() {
    delete g_scene;
    delete g_bits;
    delete g_demos;
    delete g_nav;
    delete g_sc;
    delete g_di;
    delete g_net;
}

VRGuiManager* VRGuiManager::get() {
    static VRGuiManager* instance = new VRGuiManager();
    return instance;
}

void VRGuiManager::updateGtk() {
    int N = 0;
    while( Gtk::Main::events_pending() ) {
        N++;
        Gtk::Main::iteration();
    }
    //cout << "N " << N << endl;
}

void VRGuiManager::update() {
    g_scene->update();
}

OSG_END_NAMESPACE;
