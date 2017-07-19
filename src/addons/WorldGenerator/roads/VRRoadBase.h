#ifndef VRROADBASE_H_INCLUDED
#define VRROADBASE_H_INCLUDED

#include <map>
#include <string>
#include <vector>
#include <OpenSG/OSGVector.h>
#include "../VRWorldGeneratorFwd.h"
#include "core/objects/object/VRObject.h"
#include "addons/Semantics/VRSemanticsFwd.h"

using namespace std;
OSG_BEGIN_NAMESPACE;

class VRRoadBase : public VRObject {
    protected:
        VRWorldGeneratorPtr world;

    public:
        VRRoadBase(string name);
        ~VRRoadBase();

        void setWorld(VRWorldGeneratorPtr w);

        pathPtr toPath( VREntityPtr pathEntity, int resolution );
        void setupTexCoords( VRGeometryPtr geo, VREntityPtr way );
        vector<string> toStringVector(Vec3d& v);

        VREntityPtr addNode( Vec3d pos );
        VREntityPtr addPath( string type, string name, vector<VREntityPtr> nodes, vector<Vec3d> normals );
        VREntityPtr addLane( int direction, float width );

        VRGeometryPtr addPole( Vec3d root, Vec3d end, float radius );
};

OSG_END_NAMESPACE;

#endif // VRROADBASE_H_INCLUDED
