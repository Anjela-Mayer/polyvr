#include "ModuleBuildings.h"

#include "Building.h"
#include "addons/WorldGenerator/GIS/OSMMap.h"
#include "core/objects/material/VRShader.h"
#include "core/objects/geometry/VRPhysics.h"
#include "core/objects/geometry/VRGeometry.h"
#include "core/objects/geometry/VRGeoData.h"
#include "core/objects/material/VRMaterial.h"
#include "core/utils/toString.h"
#include "core/utils/system/VRSystem.h"
#include "triangulate.h"
#include "../Config.h"
#include "../World.h"
#include "core/scene/VRSceneManager.h"
#include "../RealWorld.h"
#include "../MapCoordinator.h"

using namespace OSG;

ModuleBuildings::ModuleBuildings(bool t, bool p) : BaseModule("ModuleBuildings", t,p) {
    b_mat = VRMaterial::create("Buildings");
    string tex = "world/textures/Buildings.png";
    if (exists(tex)) b_mat->setTexture(tex, false);
    b_mat->setAmbient(Color3f(0.7, 0.7, 0.7)); //light reflection in all directions
    b_mat->setDiffuse(Color3f(1.0, 1.0, 1.0)); //light from ambient (without lightsource)
    b_mat->setSpecular(Color3f(0.2, 0.2, 0.2)); //light reflection in camera direction

    string wdir = VRSceneManager::get()->getOriginalWorkdir();
    b_mat->readVertexShader(wdir+"/shader/TexturePhong/phong.vp");
    b_mat->readFragmentShader(wdir+"/shader/TexturePhong/phong_building.fp"); //Fragment Shader
    b_mat->setMagMinFilter(GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, 0);
}

void ModuleBuildings::loadBbox(MapGrid::Box bbox) {
    auto mc = RealWorld::get()->getCoordinator();
    if (!mc) return;
    OSMMapPtr osmMap = RealWorld::get()->getMap(bbox.str);
    if (!osmMap) return;

    VRGeoData b_geo_d;
    VRGeoData r_geo_d;

    cout << "LOADING BUILDINGS FOR " << bbox.str << "\n" << flush;

    for(auto way : osmMap->getWays()) {
        if (way.second->tags["building"] != "yes") continue;
        //if (meshes.count(way.second->id)) continue;

        // load building from osmMap
        Building* b = new Building(way.second->id);
        for(string nID : way.second->nodes) {
            OSMNodePtr node = osmMap->getNode(nID);
            Vec2d pos = mc->realToWorld(Vec2d(node->lat, node->lon));
            b->positions.push_back(pos);
        }

        if (b->positions.size() < 3) continue;

        // generate mesh
        b->makeClockwise();
        makeBuildingGeometry(&b_geo_d, &r_geo_d, b);
    }

    VRGeometryPtr b_geo = b_geo_d.asGeometry("Buildings");
    VRGeometryPtr r_geo = r_geo_d.asGeometry("Roofs");
    root->addChild(b_geo);
    root->addChild(r_geo);

    //b_geo->setTexCoords(b_geo_d.texs2, 1);
    b_geo->setMaterial(b_mat);
    b_geos[bbox.str] = b_geo;

    //r_geo->setTexCoords(r_geo_d->texs2, 1);
    r_geo->setMaterial(b_mat);
    r_geos[bbox.str] = r_geo;

    if (doPhysicalize) { // TODO: crash in threaded mode!!
        b_geo->getPhysics()->setShape("Concave");
        b_geo->getPhysics()->setPhysicalized(true);
    }
}

void ModuleBuildings::unloadBbox(MapGrid::Box bbox) {
    string id = bbox.str;
    if (b_geos.count(id)) { b_geos[id]->destroy(); b_geos.erase(id); }
    if (r_geos.count(id)) { r_geos[id]->destroy(); r_geos.erase(id); }
}

void ModuleBuildings::addBuildingWallLevel(VRGeoData* b_geo_d, Vec2d pos1, Vec2d pos2, int level, int bNum, float elevation) {
    srand(bNum); //seed for random windows
    float len = (pos2 - pos1).length();
    Vec2d wallDir = (pos2 - pos1);
    wallDir.normalize();

    float wall_segment = Config::get()->WINDOW_DOOR_WIDTH;
    float FLOOR_HEIGHT = Config::get()->BUILDING_FLOOR_HEIGHT;

    int segN = floor(len / wall_segment);
    segN = max(segN, 1);
    wall_segment = len / segN;

    float low = level * FLOOR_HEIGHT + elevation;
    float high = low + FLOOR_HEIGHT;

    // insert a door at a random place (when on level 0 && there is enough room)
    int doorIndex = -1;
    if (level == 0 && segN > 2) doorIndex = bNum % segN;

    int N = 4;
    float _N = 1./N;
    float e = 0.01;

    int di = N*float(rand()) / RAND_MAX;
    int wi = N*float(rand()) / RAND_MAX;
    int fi = N*float(rand()) / RAND_MAX;

    float d_tc1 = di * _N + e;
    float d_tc2 = di * _N - e + _N;
    float w_tc1 = wi * _N + e;
    float w_tc2 = wi * _N - e + _N;
    float f_tc1 = fi * _N + e;
    float f_tc2 = fi * _N - e + _N;

    for (int i=0; i<segN; i++) {
        Vec2d w1 = pos1 + (wallDir * (i*wall_segment));
        Vec2d w2 = pos1 + (wallDir * ((i+1)*wall_segment));

        Vec2d wallVector = w2-w1;
        Vec3d n = Vec3d(-wallVector[1], 0, wallVector[0]);

        if (i == doorIndex) { // door
            int Va = b_geo_d->pushVert(Vec3d(w1[0], low, w1[1]), n, Vec2d(f_tc1, e), Vec2d(d_tc1, 0.5+e));
            int Vb = b_geo_d->pushVert(Vec3d(w2[0], low, w2[1]), n, Vec2d(f_tc2, e), Vec2d(d_tc2, 0.5+e));
            int Vc = b_geo_d->pushVert(Vec3d(w2[0], high, w2[1]), n, Vec2d(f_tc2, 0.25-e), Vec2d(d_tc2, 0.75-e));
            int Vd = b_geo_d->pushVert(Vec3d(w1[0], high, w1[1]), n, Vec2d(f_tc1, 0.25-e), Vec2d(d_tc1, 0.75-e));
            b_geo_d->pushQuad(Va, Vb, Vc, Vd);
        } else { // window
            int Va = b_geo_d->pushVert(Vec3d(w1[0], low, w1[1]), n, Vec2d(f_tc1, e), Vec2d(w_tc1, 0.25+e));
            int Vb = b_geo_d->pushVert(Vec3d(w2[0], low, w2[1]), n, Vec2d(f_tc2, e), Vec2d(w_tc2, 0.25+e));
            int Vc = b_geo_d->pushVert(Vec3d(w2[0], high, w2[1]), n, Vec2d(f_tc2, 0.25-e), Vec2d(w_tc2, 0.5-e));
            int Vd = b_geo_d->pushVert(Vec3d(w1[0], high, w1[1]), n, Vec2d(f_tc1, 0.25-e), Vec2d(w_tc1, 0.5-e));
            b_geo_d->pushQuad(Va, Vb, Vc, Vd);
        }

    }
}

Vec2d convRTC(float u, float v, Vec2d m) {
    Vec2d tc(u,v);
    tc -= m;
    float f = 0.003;
    tc *= f;
    tc += Vec2d(0.125, 0.875);
    //u *= f;
    //v *= f;
    //Vec2d tc(u-0.25*floor(4*u), 0.75+v-0.25*floor(4*v));
    //return tc;
    //cout << "AA " << Vec2d(u, v)-m << endl;
    return tc;
}

void ModuleBuildings::addBuildingRoof(VRGeoData* r_geo_d, Building* building, float height, float elevation){
    //create && fill vector a with VRPolygon corners
    Vector2dVector a;
    bool first = true;
    for(auto corner : building->getCorners()) {
        if (first) { first=false; continue; }
        a.push_back( Vector2d(corner[0], corner[1]));
    }

    Vector2dVector result; // allocate an STL vector to hold the answer.
    Triangulate::Process(a,result); //  Invoke the triangulator to triangulate this VRPolygon.

    //create roof
    height = (float)height * Config::get()->BUILDING_FLOOR_HEIGHT + elevation;
    int tcount = result.size()/3;
    for (int i=0; i<tcount; i++) {
        const Vector2d &p1 = result[i*3+0];
        const Vector2d &p2 = result[i*3+1];
        const Vector2d &p3 = result[i*3+2];

        float mx = (p1[0] + p2[0] + p3[0] )/3.0;
        float my = (p1[1] + p2[1] + p3[1] )/3.0;
        Vec3d n = Vec3d(0, 1, 0);

        r_geo_d->pushVert(Vec3d(p1[0], height, p1[1]), n, convRTC(p1[0], p1[1], Vec2d(mx,my)));
        r_geo_d->pushVert(Vec3d(p2[0], height, p2[1]), n, convRTC(p2[0], p2[1], Vec2d(mx,my)));
        r_geo_d->pushVert(Vec3d(p3[0], height, p3[1]), n, convRTC(p3[0], p3[1], Vec2d(mx,my)));
        r_geo_d->pushTri();
    }
}

/** create one Building **/
void ModuleBuildings::makeBuildingGeometry(VRGeoData* b_geo_d, VRGeoData* r_geo_d, Building* b) {
    auto mc = RealWorld::get()->getCoordinator();
    if (!mc) return;
    int bNum = toInt(b->id);
    int height = bNum % Config::get()->MAX_FLOORS + 2;
    float minElevation = 99999.0f;

    for(auto corner : b->getCorners()){
        float cornerElevation = mc->getElevation(corner);
        if(cornerElevation < minElevation) minElevation = cornerElevation;
    }

    for(auto side : b->getSides()) {
        for (int i=0; i<height; i++) {
            addBuildingWallLevel(b_geo_d, side[0], side[1], i, bNum, minElevation);
        }
    }

    addBuildingRoof(r_geo_d, b, height, minElevation);
}
