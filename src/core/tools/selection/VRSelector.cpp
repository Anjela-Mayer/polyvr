#include "VRSelector.h"
#include "core/objects/material/VRMaterial.h"
#include "core/objects/geometry/VRGeometry.h"
#include "core/objects/geometry/OSGGeometry.h"
#include "core/scene/VRScene.h"

#include <OpenSG/OSGGeometry.h>
#include <OpenSG/OSGGeoProperties.h>

OSG_BEGIN_NAMESPACE;
using namespace std;

VRSelector::VRSelector() {
    color = Color3f(0.2, 0.65, 0.9);
    selection = VRSelection::create();
}

VRSelectorPtr VRSelector::create() { return VRSelectorPtr( new VRSelector() ); }

VRSelector::MatStore::MatStore(VRGeometryPtr geo) {
    this->geo = geo;
    mat = geo->getMaterial();
}

void VRSelector::update() {
    deselect();
    if (!selection) return;

    // highlight selected objects
    for (auto g : selection->getSelected()) {
        auto geo = g.lock();
        if (!geo) continue;
        if (!geo->getMaterial()) continue;
        orig_mats.push_back(MatStore(geo));

        VRMaterialPtr mat = getMat();
        mat->prependPasses(geo->getMaterial());
        mat->setActivePass(0);
        mat->setStencilBuffer(true, 1,-1, GL_ALWAYS, GL_KEEP, GL_KEEP, GL_REPLACE);
        geo->setMaterial(mat);
    }

    // visualise subselections
    subselection = VRGeometry::create("subsel");
    subselection->setPersistency(0);
    for (auto m : selection->getSubselections()) {
        if (!m.first) continue;
        auto s = m.first->copySelection(selection);
        s->setWorldMatrix(m.first->getWorldMatrix());
        subselection->merge(s);
    }

    if (!subselection->getMesh()->geo) return;
    if (!subselection->getMesh()->geo->getPositions()) return;
    int N = subselection->getMesh()->geo->getPositions()->size();

    GeoUInt32PropertyMTRecPtr inds = GeoUInt32Property::create();
    GeoUInt32PropertyMTRecPtr lengths = GeoUInt32Property::create();
    lengths->addValue(N);
    for (int i=0; i<N; i++) inds->addValue(i);

    subselection->setType(GL_POINTS);
    subselection->setLengths(lengths);
    subselection->setIndices(inds);

    auto m = VRMaterial::create("sel");
    m->setLit(false);
    m->setDiffuse(color);
    m->setFrontBackModes(GL_LINE, GL_LINE);
    m->setPointSize(3);
    subselection->setMaterial(m);

    auto scene = VRScene::getCurrent();
    if (!scene) return;
    scene->getRoot()->addChild(subselection);
}

VRMaterialPtr VRSelector::getMat() {
    VRMaterialPtr mat = VRMaterial::create("VRSelector");

    // stencil buffer
    /*mat->setFrontBackModes(GL_POINT, GL_POINT);
    mat->setDiffuse(color);
    mat->setPointSize(8);
    mat->setLit(false);
    mat->setStencilBuffer(false, 1,-1, GL_NOTEQUAL, GL_KEEP, GL_KEEP, GL_REPLACE);

    mat->addPass();*/

    mat->setFrontBackModes(GL_LINE, GL_LINE);
    mat->setDiffuse(color);
    mat->setLineWidth(width, smooth);
    mat->setLit(false);
    mat->setStencilBuffer(false, 1,-1, GL_NOTEQUAL, GL_KEEP, GL_KEEP, GL_REPLACE);

    //mat->addPass();
    //mat->setDiffuse(color);
    //mat->setLit(false);

    return mat;
}

void VRSelector::deselect() {
    for (auto ms : orig_mats) if ( auto geo = ms.geo.lock() ) geo->setMaterial(ms.mat);
    orig_mats.clear();
    if (subselection) subselection->destroy();
    subselection.reset();
}

void VRSelector::clear() { selection->clear(); update(); }

void VRSelector::setBorder(int width, bool smooth) {
    this->width = width;
    this->smooth = smooth;
    update();
}

void VRSelector::select(VRObjectPtr obj, bool append, bool recursive) {
    selected = obj;
    if (!append) {
        clear();
        selection->apply(obj, true, recursive);
    } else {
        auto s = VRSelection::create();
        s->apply(obj, true, recursive);
        selection->append(s);
    }
    update();
}

VRObjectPtr VRSelector::getSelected() {
    if (selected) return selected;
    auto objs = selection->getSelected();
    if (objs.size() > 0) return objs[0].lock();
    return 0;
}

void VRSelector::set(VRSelectionPtr s) {
    if (s != selection) {
        clear();
        selection = s;
    }
    selected = 0;
    update();
}

void VRSelector::add(VRSelectionPtr s) {
    if (!selection) selection = s;
    else selection->append(s);
    selected = 0;
    update();
}

void VRSelector::setColor(Color3f c) { color = c; }
VRSelectionPtr VRSelector::getSelection() { return selection; }

OSG_END_NAMESPACE;
