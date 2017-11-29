#include "VRAvatar.h"
#include <OpenSG/OSGLineChunk.h>
#include <OpenSG/OSGSimpleMaterial.h>
#include <OpenSG/OSGSimpleGeometry.h>        // Methods to create simple geos.
#include "core/objects/geometry/VRGeometry.h"
#include "core/objects/geometry/OSGGeometry.h"
#include "core/objects/VRTransform.h"
#include "core/objects/material/VRMaterial.h"

OSG_BEGIN_NAMESPACE;
using namespace std;


VRObjectPtr VRAvatar::initRay() {
    VRGeometryPtr ray = VRGeometry::create("av_ray");

    vector<Vec3d> pos, norms;
    vector<Vec2d> texs;
    vector<int> inds;

    //pos.push_back(Vec3d(0,0,-5));
    //pos.push_back(Vec3d(0,0,50));
    pos.push_back(Vec3d(0,0,0));
    pos.push_back(Vec3d(0,0,-50));

    for (int i=0;i<2;i++) {
        norms.push_back(Vec3d(0,0,-1));
        inds.push_back(i);
        texs.push_back(Vec2d(0,0));
    }

    VRMaterialPtr mat = VRMaterial::get("yellow_ray");
    mat->setLineWidth(6);
    mat->setDiffuse(Color3f(1,1,0));
    mat->setAmbient(Color3f(1,1,0));
    mat->setSpecular(Color3f(1,1,0));
    mat->setLit(0);

    ray->create(GL_LINES, pos, norms, inds, texs);
    ray->setMaterial(mat);

    return ray;
}

VRObjectPtr VRAvatar::initCone() {
    VRGeometryPtr cone = VRGeometry::create("av_cone");
    cone->setMesh( OSGGeometry::create(makeConeGeo(0.3, 0.03, 32, true, true)) );
    cone->setFrom(Vec3d(0,0,-0.1));
    cone->setOrientation(Vec3d(1,0,-0.1), Vec3d(0,0,-1));

    return cone;
}

VRObjectPtr VRAvatar::initBroadRay() {//path?
    VRGeometryPtr geo = VRGeometry::create("av_broadray");
    //geo->setMesh(VRSceneLoader::get()->loadWRL("mod/flystick/fly2_w_ray.wrl"));

    return geo;
}

void VRAvatar::addAll() {
    map<string, VRObjectPtr>::iterator itr = avatars.begin();
    for(;itr != avatars.end();itr++) deviceRoot->addChild(itr->second);
}

void VRAvatar::hideAll() {
    map<string, VRObjectPtr>::iterator itr = avatars.begin();
    for(;itr != avatars.end();itr++) itr->second->hide();
}

void VRAvatar::addAvatar(VRObjectPtr geo) {
    deviceRoot->addChild(geo);
}

VRAvatar::VRAvatar(string name) {
    deviceRoot = VRTransform::create(name + "_beacons");
    deviceRoot->setPersistency(0);
    addBeacon();
    tmpContainer = VRTransform::create(name + "_tmp_beacon");
    tmpContainer->setPersistency(0);

    avatars["ray"] = initRay();
    //avatars["cone"] = initCone();
    //avatars["broadray"] = initBroadRay();

    addAll();
    hideAll();
}

VRAvatar::~VRAvatar() {}

void VRAvatar::enableAvatar(string avatar) { if (avatars.count(avatar)) avatars[avatar]->show(); }
void VRAvatar::disableAvatar(string avatar) { if (avatars.count(avatar)) avatars[avatar]->hide(); }


Beacon* VRAvatar::addBeacon() { beacons.push_back(Beacon{0, 0}); return beacons.back()}
VRTransformPtr VRAvatar::getBeacon(int i) { beacons[i].beacon; }
VRTransformPtr VRAvatar::editBeacon(int i) { return beacons[i].tmpContainer?beacons[i].tmpContainer:beacons[i].beacon; }
void VRAvatar::setBeacon(VRTransformPtr b, int i) {
    beacons[i].beacon = b;
    beacons[i].tmpContainer = 0;
    for (auto a : beacons[i].avatars) a.second->switchParent(b);
}

void VRAvatar::updateBeacons() {
    for (auto b : beacons) b.beacon->updateTransform(b.tmpContainer);
}

OSG_END_NAMESPACE;
