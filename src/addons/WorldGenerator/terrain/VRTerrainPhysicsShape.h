#ifndef VRTERRAINPHYSICSSHAPE_H_INCLUDED
#define VRTERRAINPHYSICSSHAPE_H_INCLUDED

#include "../VRWorldGeneratorFwd.h"
#include <vector>
#include <memory>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGVector.h>
#include <BulletCollision/CollisionShapes/btConcaveShape.h>

using namespace std;
OSG_BEGIN_NAMESPACE;

ATTRIBUTE_ALIGNED16(class)  VRTerrainPhysicsShape : public btConcaveShape {
    protected:
        VRTerrainPtr terrain;
        btVector3 m_localAabbMin;
        btVector3 m_localAabbMax;
        btVector3 m_localOrigin;

        int	m_heightStickWidth;
        int m_heightStickLength;
        btScalar m_width;
        btScalar m_length;

        btVector3 m_localScaling;
        Vec3i quantizeWithClamp(const btVector3& point) const;
        btVector3 getVertex(int x, int y) const;

    public:
        BT_DECLARE_ALIGNED_ALLOCATOR();

        VRTerrainPhysicsShape(VRTerrainPtr terrain);
        ~VRTerrainPhysicsShape();

        virtual void getAabb(const btTransform& t,btVector3& aabbMin,btVector3& aabbMax) const;
        virtual void processAllTriangles(btTriangleCallback* callback,const btVector3& aabbMin,const btVector3& aabbMax) const;
        virtual void calculateLocalInertia(btScalar mass,btVector3& inertia) const;
        virtual void setLocalScaling(const btVector3& scaling);
        virtual const btVector3& getLocalScaling() const;
        virtual const char*	getName()const {return "HEIGHTFIELD";}
};

OSG_END_NAMESPACE;

#endif // VRTERRAINPHYSICSSHAPE_H_INCLUDED
