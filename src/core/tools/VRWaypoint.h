#ifndef VRWAYPOINT_H_INCLUDED
#define VRWAYPOINT_H_INCLUDED

#include "core/objects/geometry/VRGeometry.h"
#include "core/tools/VRToolsFwd.h"
#include "core/math/VRMathFwd.h"

OSG_BEGIN_NAMESPACE;

class VRWaypoint : public VRGeometry {
    private:
        PosePtr Pose;
        PosePtr Floor;
        Vec3d at;
        float size;

        void setup();
        void updateGeo();

    public:
        VRWaypoint(string name);

        static VRWaypointPtr create(string name = "None");
        VRWaypointPtr ptr();

        void set(PosePtr p);
        void set(VRTransformPtr t);
        void apply(VRTransformPtr t);
        PosePtr get();

        void setSize(float s);
        void setFloorPlane(PosePtr p);
};

OSG_END_NAMESPACE;

#endif // VRWAYPOINT_H_INCLUDED
