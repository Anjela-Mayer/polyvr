#include "VRNature.h"
#include "VRTree.h"
#include "VRGrassPatch.h"
#include "../terrain/VRTerrain.h"

#include "core/scene/VRSceneManager.h"
#include "core/objects/object/VRObject.h"
#include "core/objects/geometry/VRGeometry.h"
#include "core/objects/geometry/VRGeoData.h"
#include "core/objects/geometry/VRPhysics.h"
#include "core/objects/material/VRMaterial.h"
#include "core/objects/material/VRTextureGenerator.h"
#include "core/objects/VRLod.h"
#include "core/math/Octree.h"
#include "core/math/pose.h"
#include "core/math/polygon.h"
#include "core/math/boundingbox.h"
#include "core/math/triangulator.h"
#include "core/utils/toString.h"
#include "core/utils/VRStorage_template.h"
#include "core/utils/VRTimer.h"

using namespace OSG;

VRLodLeaf::VRLodLeaf(string name, Octree* o, int l) : VRTransform(name), oLeaf(o), lvl(l) {}
VRLodLeaf::~VRLodLeaf() {}
VRLodLeafPtr VRLodLeaf::ptr() { return static_pointer_cast<VRLodLeaf>( shared_from_this() ); }

VRLodLeafPtr VRLodLeaf::create(string name, Octree* o, int lvl) {
    auto l = VRLodLeafPtr(new VRLodLeaf(name, o, lvl));
    l->lod = VRLod::create("lod");
    l->lod->setPersistency(0);
    l->addChild(l->lod);
    auto lvl0 = VRObject::create("lvl");
    lvl0->setPersistency(0);
    l->levels.push_back(lvl0);
    l->lod->addChild(lvl0);
    return l;
}

void VRLodLeaf::addLevel(float dist) {
    auto lvl = VRObject::create("lvl");
    lvl->setPersistency(0);
    levels.push_back(lvl);
    lod->addChild(lvl);
    lod->addDistance(dist);
}

void VRLodLeaf::add(VRObjectPtr obj, int lvl) {
    levels[lvl]->addChild(obj);
}

void VRLodLeaf::set(VRObjectPtr obj, int lvl) {
    if (lvl < 0 || lvl >= int(levels.size())) return;
    levels[lvl]->clearChildren();
    if (obj) levels[lvl]->addChild(obj);
}

Octree* VRLodLeaf::getOLeaf() { return oLeaf; }
int VRLodLeaf::getLevel() { return lvl; }

// --------------------------------------------------------------------------------------------------

VRLodTree::VRLodTree(string name, float size) : VRObject(name) { octree = Octree::create(size,size); }
VRLodTree::~VRLodTree() {}
VRLodTreePtr VRLodTree::ptr() { return static_pointer_cast<VRLodTree>( shared_from_this() ); }
VRLodTreePtr VRLodTree::create(string name) { return VRLodTreePtr(new VRLodTree(name)); }

void VRLodTree::reset(float size) {
    leafs.clear();
    objects.clear();
    rootLeaf = 0;
    if (size > 0) octree = Octree::create(size,size);
    else {
        auto s = octree->getSize();
        octree = Octree::create(s,s);
    }
    clearChildren();
}

vector<VRLodLeafPtr> VRLodTree::getSubTree(VRLodLeafPtr l) {
    vector<VRLodLeafPtr> res;
    if (!l->getOLeaf()) return res;
    for (auto o : l->getOLeaf()->getSubtree()) {
        if (!leafs.count(o)) continue;
        res.push_back(leafs[o]);
    }
    return res;
}

VRLodLeafPtr VRLodTree::addLeaf(Octree* o, int lvl) {
    if (leafs.count(o)) return leafs[o];
    auto l = VRLodLeaf::create("lodLeaf", o, lvl);
    l->setPersistency(0);
    if (lvl > 0) l->addLevel( o->getSize()*5 );
    l->setFrom(o->getLocalCenter());
    leafs[o] = l;

    /**
    add lod leaf to tree, handle following cases:
        - octree leaf has parent
            - parent lod leaf does not exist
                -> call addLeaf on the parent octree leaf, may result in further recursion
            - parent lod leaf exists
                -> pass
            (at this point the leaf parent should exist!)
            -> add leaf to leaf parent
        - octree leaf has no parent
            - first lod leaf (no root leaf set), created in tree constructor
                -> pass
            - tree grows bigger, new root leaf
                -> switch parent of old root leaf to new root leaf
                -> update local position of old root leaf
            -> remember as root leaf
            -> add as child to tree
    */

    VRLodLeafPtr oldRootLeaf = 0;
    if (auto p = o->getParent()) {
        if (!leafs.count(p)) addLeaf(p, lvl+1);
        leafs[p]->add(l,0);
    } else {
        if (rootLeaf) oldRootLeaf = rootLeaf;
        rootLeaf = l;
        addChild(l);
    }

    if (oldRootLeaf) { // TODO: find pl
        auto p = oldRootLeaf->getOLeaf()->getParent();
        if (!leafs.count(p)) addLeaf(p, lvl+1);
        leafs[p]->add(oldRootLeaf,0);
        oldRootLeaf->setFrom( oldRootLeaf->getOLeaf()->getLocalCenter() );
    }

    return l;
}

VRLodLeafPtr VRLodTree::addObject(VRTransformPtr obj, Vec3d p, int lvl) {
    if (leafs.size() == 0) addLeaf(octree.get(), 0);
    if (!octree) return 0;
    objects[lvl].push_back(obj);
    auto oLeaf = octree->add(p, obj.get(), lvl, 0, true);
    auto leaf = addLeaf(oLeaf, lvl);
    if (lvl == 0) leaf->add(obj, 0);
    else          leaf->add(obj, 1);
    obj->setRelativePosition(p, ptr());
    obj->setDir(Vec3d(0,0,-1));
    obj->setUp(Vec3d(0,1,0));
    return leaf;
}

VRLodLeafPtr VRLodTree::remObject(VRTransformPtr obj) { // TODO, finish it!
    if (!octree) return 0;
    Octree* o = octree->get( obj->getWorldPosition() );
    o->remData(obj.get());
    return leafs[o];
}

// --------------------------------------------------------------------------------------------------

VRNature::VRNature(string name) : VRLodTree(name, 5) {
    storeMap("templateTrees", &treeTemplates, true);
    storeMap("trees", &treeEntries, true);
    regStorageSetupFkt( VRUpdateCb::create("woods setup", boost::bind(&VRNature::setup, this)) );
}

VRNature::~VRNature() {}
VRNaturePtr VRNature::create(string name) { return VRNaturePtr(new VRNature(name)); }
VRNaturePtr VRNature::ptr() { return static_pointer_cast<VRNature>( shared_from_this() ); }

void VRNature::setup() {
    for (auto& t : treeEntries) {
        if (!treeTemplates.count(t.second->type)) { cout << "VRNature::setup Warning, " << t.second->type << " is not a tree template!" << endl; continue; }
        auto tree = treeTemplates[t.second->type];
        tree->setPose(t.second->pos);
        addTree(tree, 0, false);
    }
    computeLODs();
}

VRTreePtr VRNature::getTree(int id) {
    if (treesByID.count(id)) return treesByID[id];
    return 0;
}

void VRNature::remTree(int id) {
    if (!treesByID.count(id)) return;
    auto t = treesByID[id];
    auto leaf = remObject(t);
    treesByID.erase(id);
    treeRefs.erase(t.get());
    treeEntries.erase(t->getName());
    t->destroy();

    auto oLeafs = leaf->getOLeaf()->getAncestry();
    map<Octree*, VRLodLeafPtr> aLeafs;
    for (auto o : oLeafs) {
        if (leafs.count(o) == 0) continue;
        aLeafs[o] = leafs[o];
    }
    computeLODs(aLeafs);
}

void VRNature::addGrassPatch(VRPolygonPtr Area, bool updateLODs, bool addGround, bool addKirb) { // TODO: needs optimizations!
    VRTimer timer; timer.start();
    int t0 = timer.stop();
    //cout << "VRNature::addGrassPatch " << t0 << endl;
    int i=0;
    auto ground = VRGeometry::create("ground");

    for (auto area : Area->gridSplit(10.0)) {
        //cout << " sub Area " << i << "  " << timer.stop() - t0 << endl;
        Vec3d median = area->getBoundingBox().center();
        if (terrain) terrain->elevatePoint(median); // TODO: elevate each point of the polygon
        area->translate(-median);
        //cout << "  A1 " << timer.stop() - t0 << endl;
        auto grass = VRGrassPatch::create();
        grass->addAttachment("grass", 0);
        grass->setArea(area);
        //cout << "  A2 " << timer.stop() - t0 << endl;
        grassPatchRefs[grass.get()] = grass;
        auto leaf = addObject(grass, median, 0); // pose contains the world position!
        grass->setWorldPosition(median);
        if (updateLODs) computeLODs(leaf);
        //cout << "  A3 " << timer.stop() - t0 << endl;

        if (addGround) {
            Triangulator tri;
            tri.add(*area);
            auto geo = tri.compute();
            geo->rotateYonZ();
            geo->translate(Vec3d(0,0.02,0) + median);
            ground->merge(geo);
        }

        i++;
    }

    if (addGround) {
        VRTextureGenerator tg;
        tg.addSimpleNoise( Vec3i(128,128,1), true, Color4f(0.85,0.8,0.75,1), Color4f(0.5,0.3,0,1) );
        auto mat = VRMaterial::create("earth");
        mat->setTexture(tg.compose());
        ground->setMaterial(mat);
        ground->setPositionalTexCoords(1.0, 0, Vec3i(0,2,1));
        addChild(ground);
    }
}

VRTreePtr VRNature::addTree(VRTreePtr t, bool updateLODs, bool addToStore) {
    posePtr p = t->getRelativePose(ptr());
    if (terrain) terrain->elevatePose(p);

    auto tree = dynamic_pointer_cast<VRTree>( t->duplicate() );
    tree->addAttachment("tree", 0);
    treeTemplates[t->getName()] = t;
    treeRefs[tree.get()] = t;
    auto leaf = addObject(tree, p->pos(), 0); // pose contains the world position!
    treesByID[tree->getID()] = tree;

    auto te = VRObjectManager::Entry::create();
    te->set( p, t->getName());
    if (addToStore) treeEntries[tree->getName()] = te;
    if (updateLODs) computeLODs(leaf);
    return tree;
}

void VRNature::computeLODs() { computeLODs(leafs); }

void VRNature::computeLODs(VRLodLeafPtr leaf) {
    auto oLeafs = leaf->getOLeaf()->getAncestry();
    map<Octree*, VRLodLeafPtr> aLeafs;
    for (auto o : oLeafs) {
        if (leafs.count(o) == 0) continue;
        aLeafs[o] = leafs[o];
    }
    computeLODs(aLeafs);
}

void VRNature::computeLODs(map<Octree*, VRLodLeafPtr>& leafs) {
    auto simpleLeafMat = [](bool doAlpha) {
        auto m = VRMaterial::create("simpleLeafMat");
        m->setPointSize(3);
        //m->setDiffuse(Color3f(0.5,1,0));
        m->setDiffuse(Color3f(0,0,1));
        m->setAmbient(Color3f(0.1,0.3,0));
        m->setSpecular(Color3f(0.1,0.4,0));
        string wdir = VRSceneManager::get()->getOriginalWorkdir();
        m->readFragmentShader(wdir+"/shader/Trees/Shader_leafs_lod.fp");
        m->readVertexShader(wdir+"/shader/Trees/Shader_leafs_lod.vp");

        auto tg = VRTextureGenerator::create();
        tg->setSize(Vec3i(50,50,50), doAlpha);
        float r = 0.85;
        float g = 1.0;
        float b = 0.8;
        tg->add(PERLIN, 1, Color4f(r,g,b,0.9), Color4f(r,g,b,1) );
        tg->add(PERLIN, 0.5, Color4f(r,g,b,0.5), Color4f(r,g,b,1) );
        tg->add(PERLIN, 0.25, Color4f(r,g,b,0.8), Color4f(r,g,b,1) );
        m->setTexture(tg->compose(0));

        return m;
    };

    auto simpleTrunkMat = []() {
        auto m = VRMaterial::create("brown");
        m->setDiffuse(Color3f(0.6,0.3,0));
        string wdir = VRSceneManager::get()->getOriginalWorkdir();
        m->readFragmentShader(wdir+"/shader/Trees/Shader_trunc_lod.fp");
        m->readVertexShader(wdir+"/shader/Trees/Shader_trunc_lod.vp");
        return m;
    };

    auto simpleGrassMat = []() {
        return VRGrassPatch::getGrassSideMaterial();
    };

    // get all trees and grass patches for each leaf layer
    map<VRLodLeaf*, vector<VRTree*> > trees;
    map<VRLodLeaf*, vector<VRGrassPatch*> > grass;
    for (auto l : leafs) {
        auto& leaf = l.second;
        int lvl = leaf->getLevel();
        if (lvl == 0) continue;

        vector<void*> data = leaf->getOLeaf()->getAllData();
        for (auto v : data) {
            if (((VRObject*)v)->hasTag("tree")) trees[leaf.get()].push_back((VRTree*)v);
            if (((VRObject*)v)->hasTag("grass")) grass[leaf.get()].push_back((VRGrassPatch*)v);
        }
    }

    // create layer node geometries
    for (auto l : leafs) {
        auto& leaf = l.second;
        leaf->set( 0, 1 );
        int lvl = leaf->getLevel();
        if (lvl == 0) continue;
        bool doTrees = (trees.count(leaf.get()) >= 0);
        bool doGrass = (grass.count(leaf.get()) >= 0);

        Boundingbox bb;
        if (doTrees) for (auto t : trees[leaf.get()]) bb.update( t->getWorldPosition() );
        //if (doGrass) for (auto g : grass[leaf.get()]) bb.update( g->getWorldPosition() );
        Vec3d pos = bb.center();

        VRGeoData geoLeafs;
        VRGeoData geoTrunk;
        VRGeoData geoGrass;

        if (doTrees) for (auto t : trees[leaf.get()]) {
            if (treeRefs.count(t) == 0) continue;
            auto tRef = treeRefs[t];
            if (!tRef || !t) continue;
            Vec3d offset = t->getWorldPosition() - pos;
            tRef->createHullTrunkLod(geoTrunk, lvl, offset, t->getID());
            tRef->createHullLeafLod (geoLeafs, lvl, offset, t->getID());
        }

        if (doGrass) for (auto g : grass[leaf.get()]) {
            if (grassPatchRefs.count(g) == 0) continue;
            auto gRef = grassPatchRefs[g];
            if (!gRef || !g) continue;
            Vec3d offset = g->getWorldPosition() - pos;
            gRef->createLod(geoGrass, lvl, offset, g->getID());
        }

        VRGeometryPtr trunk;
        if (geoTrunk.size() > 0) {
            trunk = geoTrunk.asGeometry("trunk");
            if (!truncMat) truncMat = simpleTrunkMat();
            trunk->setMaterial(truncMat);

            leaf->set( trunk, 1 );
            trunk->setWorldPosition(pos);
            trunk->setDir(Vec3d(0,0,-1));
            trunk->setUp(Vec3d(0,1,0));

            if (geoLeafs.size() > 0) {
                auto leafs = geoLeafs.asGeometry("leafs");
                leafs->setPersistency(0);
                trunk->addChild( leafs );
                if (!leafMat1) leafMat1 = simpleLeafMat(true);
                if (!leafMat2) leafMat2 = simpleLeafMat(false);
                if (lvl < 3) leafs->setMaterial(leafMat1);
                else leafs->setMaterial(leafMat2);
            }
        }

        if (geoGrass.size() > 0) {
            auto patch = geoGrass.asGeometry("grass");
            if (!grassMat) grassMat = simpleGrassMat();
            patch->setMaterial(grassMat);

            if (trunk) trunk->addChild(patch);
            else leaf->set( patch, 1 );
            patch->setWorldPosition(pos);
            patch->setDir(Vec3d(0,0,-1));
            patch->setUp(Vec3d(0,1,0));
        }
    }
}

void VRNature::clear() {
    treesByID.clear();
    treeRefs.clear();
    treeEntries.clear();
    treeTemplates.clear();
    VRLodTree::reset();
}

void VRNature::addCollisionModels() {
    VRGeoData data;

    for (auto tree : treesByID) {
        auto p = tree.second->getPoseTo( ptr() );
        data.pushQuad(p->pos()+Vec3d(0,1,0), p->dir(), p->up(), Vec2d(0.3, 2), true);
    }

    if (collisionMesh) collisionMesh->destroy();
    collisionMesh = data.asGeometry("treeCollisionMesh");
    collisionMesh->getPhysics()->setDynamic(false);
    collisionMesh->getPhysics()->setShape("Concave");
    collisionMesh->getPhysics()->setPhysicalized(true);
    collisionMesh->setMeshVisibility(false);
    addChild( collisionMesh );
}

/**

TODO:
 - get rid of transform of lodleaf?
 - optimize level zero structure

*/


