#include "ART.h"
#include "core/scene/VRThreadManager.h"
#include "core/setup/VRSetup.h"
#include "core/utils/VROptions.h"
#include "core/scene/VRScene.h"
#include "core/scene/VRSceneManager.h"
#include "core/utils/toString.h"
#include <libxml++/nodes/element.h>
#include "DTrack.h"
#include "core/setup/devices/VRFlystick.h"
#include "core/utils/VRFunction.h"
#include "core/objects/VRTransform.h"
#include "core/objects/VRCamera.h"
#include "core/math/coordinates.h"
#include "core/utils/VRStorage_template.h"
#include "core/setup/devices/VRSignal.h"

OSG_BEGIN_NAMESPACE;
using namespace std;

ART_device::ART_device() { setName("ARTDevice"); }
ART_device::ART_device(int ID, int type) : ID(ID), type(type) { setName("ARTDevice"); init(); }

ART_devicePtr ART_device::create(int ID, int type) { return ART_devicePtr( new ART_device(ID, type) ); }

int ART_device::key() { return key(ID, type); }
int ART_device::key(int ID, int type) { return ID*1000 + type; }

void ART_device::init() {
    if (type != 1) ent = VRTransform::create("ART_tracker");

    if (type == 2) { // finger tracking
        for (int i=0;i<5;i++) {
            auto f = VRTransform::create("finger"+toString(i));
            ent->addChild(f);
            fingerEnts.push_back(f);
        }
    }

    if (type == 1) { // flystick
        dev = VRFlystick::create();
        ent = dev->editBeacon();

        auto setup = VRSetup::getCurrent();
        if (setup) setup->addDevice(dev);

        auto scene = VRScene::getCurrent();
        if (scene) {
            scene->initFlyWalk(scene->getActiveCamera(), dev);
            scene->setActiveNavigation("FlyWalk");
            dev->clearDynTrees();
            dev->addDynTree(scene->getRoot());
        }
    }
}

void ART_device::update() {
    if (ent) {
        ent->setMatrix(m);
        if (fingers.size() == 5 && fingerEnts.size() == 5)
            for (int i=0;i<5;i++) fingerEnts[i]->setMatrix( fingers[i] );
    }

    if (dev) {
        for (auto j : joysticks) dev->update(j);
        for (auto b : buttons) dev->update(b);
        buttons.clear();
        joysticks.clear();
    }
}


ART::ART() {
    threadFkt = VRFunction< weak_ptr<VRThread> >::create("ART_fetch", boost::bind(&ART::updateT, this, _1));
    VRSceneManager::get()->initThread(threadFkt, "ART_fetch", true); // applyEvent is the sync function

    on_new_device = VRSignal::create();

    store("active", &active);
    store("port", &port);
    store("offset", &offset);
    store("up", &up);
}

ART::~ART() {
    //VRSceneManager::get()->stopThread(fetchThread);
}

template<typename dev>
void ART::getMatrix(dev t, Matrix4d& m, bool doOffset) { //TODO: its a mess :(
    m[0] = Vec4d(t.rot[0], t.rot[1], t.rot[2], 1); // orientation
    m[1] = Vec4d(t.rot[3], t.rot[4], t.rot[5], 1);
    m[2] = Vec4d(t.rot[6], t.rot[7], t.rot[8], 1);

    m[1] = Vec4d(t.rot[6], t.rot[7], t.rot[8], 1); // test
    m[2] = Vec4d(-t.rot[3], -t.rot[4], -t.rot[5], 1);

    m[3] = Vec4d(t.loc[0]*0.001, t.loc[1]*0.001, t.loc[2]*0.001, 1); // position
    coords::YtoZ(m);
    if (doOffset) m[3] += Vec4d(offset);
}

template<typename dev>
void ART::getMatrix(dev t, ART_devicePtr d) {
    if (t.quality <= 0) return;
    getMatrix(t, d->m);
    d->m[3] += Vec4d(d->offset);
}

void ART::scan(int type, int N) {
    if (type < 0) {
        boost::mutex::scoped_lock lock(mutex);
        scan(0, dtrack->get_num_body());
        scan(1, dtrack->get_num_flystick());
        scan(2, dtrack->get_num_hand());
        scan(3, dtrack->get_num_meatool());
        //scan(4, dtrack->get_num_marker());
        return;
    }

    for (int i=0; i<N; i++) {
        int k = ART_device::key(i,type);
        if (devices.count(k) == 0) continue;
        auto& dev = devices[k];

        if (type == 0) getMatrix(dtrack->get_body(i), dev);
        if (type == 1) getMatrix(dtrack->get_flystick(i), dev);
        if (type == 2) getMatrix(dtrack->get_hand(i), dev);
        if (type == 3) getMatrix(dtrack->get_meatool(i), dev);

        if (type == 1) {
            auto fly = dtrack->get_flystick(i);
            dev->buttons.push_back( vector<int>(fly.button, &fly.button[fly.num_button]) );
            dev->joysticks.push_back( vector<float>(fly.joystick, &fly.joystick[fly.num_joystick]) );
        }

        if (type == 2) {
            auto hand = dtrack->get_hand(i);
            for (int i = 0; i < hand.nfinger; i++) {
                auto finger = hand.finger[i];
                getMatrix(finger, dev->fingers[i], false);
            }
        }
    }
}

void ART::updateL() { updateT( weak_ptr<VRThread>() ); }

//update thread
void ART::updateT( weak_ptr<VRThread>  t) {
    if (!active) {
        osgSleep(1);
        return;
    }

    setARTPort(port);
    if (!active || dtrack == 0) return;

    if (dtrack->receive()) scan();
    else {
        if(dtrack->timeout())       cout << "--- ART: timeout while waiting for udp data" << endl;
        if(dtrack->udperror())      cout << "--- ART: error while receiving udp data" << endl;
        if(dtrack->parseerror())    cout << "--- ART: error while parsing udp data" << endl;
    }
}

void ART::update_setup() {
    auto setup = VRSetup::getCurrent();
    setup->updateViews(); // TODO: fuer headtracking, solte vlt wo anders hin
    auto r = setup->getRoot();
    for (auto d : devices) {
        auto b = d.second->ent;
        if (b->getParent() != r) r->addChild(b);
    }
}

void ART::checkNewDevices(int type, int N) {
    setARTPort(port);
    if (!active || dtrack == 0) return;

    //check for new devices
    if (type < 0) {
        checkNewDevices(0, dtrack->get_num_body());
        checkNewDevices(1, dtrack->get_num_flystick());
        checkNewDevices(2, dtrack->get_num_hand());
        checkNewDevices(3, dtrack->get_num_meatool());
        //checkNewDevices(4, dtrack->get_num_marker());
        return;
    }

    for (int i=0; i<N; i++) {
        int k = ART_device::key(i,type);
        if (devices.count(k) == 0) {
            cout << "ART - New device " << type << " " << k << endl;
            devices[k] = ART_device::create(i,type);
            on_new_device->triggerPtr<VRDevice>();
            update_setup();
        }
    }
}

void ART::applyEvents() {
    boost::mutex::scoped_lock lock(mutex);
    checkNewDevices();
    for (auto d : devices) d.second->update();
}

vector<int> ART::getARTDevices() {
    vector<int> devs;
    for (auto& itr : devices) devs.push_back(itr.first);
    return devs;
}

ART_devicePtr ART::getARTDevice(int dev) { return devices[dev]; }

void ART::setARTActive(bool b) { active = b; }
bool ART::getARTActive() { return active; }

int ART::getARTPort() { return port; }
void ART::setARTPort(int port) {
    if (current_port == port) return;
    this->port = port;
    current_port = port;

    if (dtrack != 0) delete dtrack;
    dtrack = new DTrack(port);
    if (!dtrack->valid()) {
        cout << "DTrack init error" << endl;
        delete dtrack;
        port = -1;
        dtrack = 0;
        return;
    }
    dtrack->receive();
}

void ART::setARTOffset(Vec3d o) { offset = o; }
Vec3d ART::getARTOffset() { return offset; }

void ART::startTestStream() {
    // TODO: create test data
}

VRSignalPtr ART::getSignal_on_new_art_device() { return on_new_device; }

OSG_END_NAMESPACE
