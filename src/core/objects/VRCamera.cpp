#include "VRCamera.h"
#include "OSGCamera.h"
#include "core/utils/toString.h"
#include "core/math/boundingbox.h"
#include "core/objects/material/VRMaterial.h"
#include "core/objects/material/OSGMaterial.h"
#include "core/objects/OSGObject.h"
#include "core/scene/VRScene.h"
#include "core/gui/VRGuiManager.h"
#include <OpenSG/OSGTransform.h>
#include <OpenSG/OSGSimpleMaterial.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGMultiPassMaterial.h>
#include <OpenSG/OSGPerspectiveCamera.h>
#include <libxml++/nodes/element.h>

OSG_BEGIN_NAMESPACE;
using namespace std;

VRMaterialPtr getCamGeoMat() {
    VRMaterialPtr mat = VRMaterial::get("cam_geo_mat");
    mat->setDiffuse(Color3f(0.9, 0.9, 0.9));
    mat->setTransparency(0.3);
    mat->setLit(false);
    return mat;
}

VRCamera::VRCamera(string name) : VRTransform(name) {
    type = "Camera";

    PerspectiveCameraMTRecPtr pcam = PerspectiveCamera::create();
    cam = OSGCamera::create( pcam );
    pcam->setBeacon(getNode()->node);
    setFov(osgDegree2Rad(60));

    store("accept_root", &doAcceptRoot);
    store("near", &nearClipPlaneCoeff);
    store("far", &farClipPlaneCoeff);
    store("aspect", &aspect);
    store("fov", &fov);
    regStorageSetupFkt( VRUpdateCb::create("camera_update", boost::bind(&VRCamera::setup, this)) );

    // cam geo
    TransformMTRecPtr trans = Transform::create();
    NodeMTRecPtr t = makeNodeFor(trans);
    trans->editMatrix().setTranslate(Vec3f(0,0,0.25));
    GeometryMTRecPtr camGeo_ = makeBoxGeo(0.2, 0.2, 0.25, 1, 1, 1); //
    GeometryMTRecPtr camGeo2_ = makeCylinderGeo(0.2, 0.07, 16, 1, 1, 1);
    camGeo = OSGObject::create( makeNodeFor(camGeo_) );
    NodeMTRecPtr camGeo2 = makeNodeFor(camGeo2_);
    camGeo->node->setTravMask(0);
    camGeo_->setMaterial(getCamGeoMat()->getMaterial()->mat);
    camGeo2_->setMaterial(getCamGeoMat()->getMaterial()->mat);
    addChild(OSGObject::create(t));
    t->addChild(camGeo->node);
    TransformMTRecPtr trans2 = Transform::create();
    NodeMTRecPtr t2 = makeNodeFor(trans2);
    trans2->editMatrix().setTranslate(Vec3f(0,0,-0.15));
    trans2->editMatrix().setRotate(Quaternion(Vec3f(1,0,0), Pi*0.5));
    camGeo->node->addChild(t2);
    t2->addChild(camGeo2);
}

VRCamera::~VRCamera() {
    VRGuiManager::broadcast("camera_added");
}

VRCameraPtr VRCamera::ptr() { return static_pointer_cast<VRCamera>( shared_from_this() ); }
VRCameraPtr VRCamera::create(string name, bool reg) {
    auto p = shared_ptr<VRCamera>(new VRCamera(name) );
    getAll().push_back( p );
    VRGuiManager::broadcast("camera_added");
    if (reg) VRScene::getCurrent()->setMActiveCamera(p->getName());
    return p;
}

void VRCamera::setup() {
    cout << "VRCamera::setup\n";
    PerspectiveCameraMTRecPtr pcam = dynamic_pointer_cast<PerspectiveCamera>(cam->cam);
    pcam->setAspect(aspect);
    pcam->setFov(fov);
    pcam->setNear(parallaxD * nearClipPlaneCoeff);
    pcam->setFar(parallaxD * farClipPlaneCoeff);
}

void VRCamera::activate() {
    auto scene = VRScene::getCurrent();
    if (scene) scene->setActiveCamera(getName());
    VRGuiManager::broadcast("camera_added");
}

void VRCamera::showCamGeo(bool b) {
    if (b) camGeo->node->setTravMask(0xffffffff);
    else camGeo->node->setTravMask(0);
}

list<VRCameraWeakPtr>& VRCamera::getAll() {
    static list<VRCameraWeakPtr> objs;
    return objs;
}

OSGCameraPtr VRCamera::getCam() { return cam; }

void VRCamera::setAcceptRoot(bool b) { doAcceptRoot = b; }
bool VRCamera::getAcceptRoot() { return doAcceptRoot; }
float VRCamera::getAspect() { return aspect; }
float VRCamera::getFov() { return fov; }
float VRCamera::getNear() { return nearClipPlaneCoeff; }
float VRCamera::getFar() { return farClipPlaneCoeff; }
void VRCamera::setAspect(float a) { aspect = a; setup(); }
void VRCamera::setFov(float f) { fov = f; setup(); }
void VRCamera::setNear(float a) { nearClipPlaneCoeff = a; setup(); }
void VRCamera::setFar(float f) { farClipPlaneCoeff = f; setup(); }
void VRCamera::setProjection(string p) {
    if (p == "perspective"); // TODO
    if (p == "orthographic"); // TODO
}

vector<string> VRCamera::getProjectionTypes() {
    vector<string> proj;
    proj.push_back("perspective");
    proj.push_back("orthographic");
    return proj;
}

void VRCamera::focus(Vec3d p) {
    setAt(p);
}

void VRCamera::focus(VRObjectPtr t) {
    auto bb = t->getBoundingbox();
    Vec3d c = bb->center();

    Vec3d d = getDir();
    focus(c);

    Vec3d dp = getDir();
    if (dp.length() > 1e-4) d = dp; // only use new dir if it is valid
    d.normalize();
    float r = max(bb->radius()*2, 0.1f);
    setFrom(c - d*r); // go back or forth to see whole node

    //cout << "VRCamera::focus " << t->getName() << " pos " << c << " size " << r << endl;
}

OSG_END_NAMESPACE;
