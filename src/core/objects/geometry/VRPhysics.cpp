#include "VRPhysics.h"
#include "core/scene/VRSceneManager.h"
#include "core/scene/VRScene.h"
#include "core/objects/geometry/VRGeometry.h"
#include "core/objects/geometry/VRConstraint.h"
#include <OpenSG/OSGTriangleIterator.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

struct VRPhysicsJoint {
    OSG::VRConstraint* constraint;
    OSG::VRConstraint* spring;
    VRPhysics* partner;
    btGeneric6DofSpringConstraint* btJoint;

    VRPhysicsJoint() {
        constraint = 0;
        spring = 0;
        partner = 0;
        btJoint = 0;
    }

    ~VRPhysicsJoint() {
        if (btJoint) delete btJoint;
        btJoint = 0;
    }

    VRPhysicsJoint(VRPhysics* p, OSG::VRConstraint* c, OSG::VRConstraint* cs) {
        constraint = c;
        spring = cs;
        partner = p;
        btJoint = 0;
    }
};

VRPhysics::VRPhysics(OSG::VRTransform* t) {
    vr_obj = t;

    world = 0;
    body = 0;
    shape = 0;
    motionState = 0;
    mass = 1.0;
    collisionMargin = 0.04;
    physicalized = false;
    dynamic = false;
    physicsShape = "Convex";
    activation_mode = ACTIVE_TAG;

    collisionGroup = 1;
    collisionMask = 1;
}

VRPhysics::~VRPhysics() {
    if (body) {
        if (world) world->removeRigidBody(body);
        delete body;
    }

    if (shape != 0) delete shape;
    if (motionState != 0) delete motionState;

    for (auto j : joints) {
        world->removeConstraint(j.second->btJoint);
        delete j.second;
    }
}

btCollisionObject* VRPhysics::getCollisionObject() { return ghost ? (btCollisionObject*)ghost_body : (btCollisionObject*)body; }
btRigidBody* VRPhysics::getRigidBody() { return body; }
btPairCachingGhostObject* VRPhysics::getGhostBody() { return ghost_body; }
btCollisionShape* VRPhysics::getCollisionShape() { return shape; }

void VRPhysics::setPhysicalized(bool b) { physicalized = b; update(); }
void VRPhysics::setShape(string s, float param) { physicsShape = s; shape_param = param; update(); }
bool VRPhysics::isPhysicalized() { return physicalized; }
string VRPhysics::getShape() { return physicsShape; }
void VRPhysics::setDynamic(bool b) { dynamic = b; update(); }
bool VRPhysics::isDynamic() { return dynamic; }
void VRPhysics::setMass(float m) { mass = m; update(); }
float VRPhysics::getMass() { return mass; }
void VRPhysics::setCollisionMargin(float m) { collisionMargin = m; update(); }
float VRPhysics::getCollisionMargin() { return collisionMargin; }
void VRPhysics::setCollisionGroup(int g) { collisionGroup = g; update(); }
void VRPhysics::setCollisionMask(int m) { collisionMask = m; update(); }
int VRPhysics::getCollisionGroup() { return collisionGroup; }
int VRPhysics::getCollisionMask() { return collisionMask; }
void VRPhysics::setActivationMode(int m) { activation_mode = m; update(); }
int VRPhysics::getActivationMode() { return activation_mode; }
void VRPhysics::setGhost(bool b) { ghost = b; update(); }
bool VRPhysics::isGhost() { return ghost; }
OSG::Vec3f VRPhysics::toVec3f(btVector3 v) { return OSG::Vec3f(v[0], v[1], v[2]); }

vector<VRCollision> VRPhysics::getCollisions() {
    vector<VRCollision> res;
    if (!physicalized) return res;

    if (!ghost) {
        int numManifolds = world->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++) {
            btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
            //btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
            //btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

            int numContacts = contactManifold->getNumContacts();
            for (int j=0;j<numContacts;j++) {
                btManifoldPoint& pt = contactManifold->getContactPoint(j);
                if (pt.getDistance()<0.f) {
                    VRCollision c;
                    c.obj1 = vr_obj;
                    // c.obj2 = // TODO
                    c.pos1 = toVec3f( pt.getPositionWorldOnA() );
                    c.pos2 = toVec3f( pt.getPositionWorldOnB() );
                    c.norm = toVec3f( pt.m_normalWorldOnB );
                    res.push_back(c);
                }
            }
        }
        return res;
    }

    // --------- ghost object --------------
    btManifoldArray   manifoldArray;
    btBroadphasePairArray& pairArray = ghost_body->getOverlappingPairCache()->getOverlappingPairArray();
    int numPairs = pairArray.size();

    for (int i=0;i<numPairs;i++) {
        manifoldArray.clear();

        const btBroadphasePair& pair = pairArray[i];

        //unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
        btBroadphasePair* collisionPair = world->getPairCache()->findPair(pair.m_pProxy0,pair.m_pProxy1);
        if (!collisionPair)
            continue;

        if (collisionPair->m_algorithm)
            collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

        for (int j=0;j<manifoldArray.size();j++) {
            btPersistentManifold* manifold = manifoldArray[j];
            btScalar directionSign = manifold->getBody0() == ghost_body ? btScalar(-1.0) : btScalar(1.0);
            for (int p=0;p<manifold->getNumContacts();p++) {
                const btManifoldPoint&pt = manifold->getContactPoint(p);
                if (pt.getDistance()<0.f) {
                    VRCollision c;
                    c.pos1 = toVec3f( pt.getPositionWorldOnA() );
                    c.pos2 = toVec3f( pt.getPositionWorldOnB() );
                    c.norm = toVec3f( pt.m_normalWorldOnB*directionSign );
                    res.push_back(c);
                }
            }
        }
    }

    return res;
}

vector<string> VRPhysics::getPhysicsShapes() {
    static vector<string> shapes;
    if (shapes.size() == 0) {
        shapes.push_back("Box");
        shapes.push_back("Sphere");
        shapes.push_back("Convex");
        shapes.push_back("Concave");
    }
    return shapes;
}

void VRPhysics::update() {
    OSG::VRScene* scene = OSG::VRSceneManager::getCurrent();
    if (scene == 0) return;

    if (world == 0) world = scene->bltWorld();
    if (world == 0) return;

    scene->unphysicalize(vr_obj);

    if (body != 0) {
        for (auto j : joints) {
            if (j.second->btJoint != 0) {
                VRPhysicsJoint* joint = j.second;
                world->removeConstraint(joint->btJoint);
                delete joint->btJoint;
                joint->btJoint = 0;
            }
        }

        for (auto j : joints2) {
            if (j.first->joints.count(this) == 0) continue;
            VRPhysicsJoint* joint = j.first->joints[this];
            if (joint->btJoint != 0) {
                world->removeConstraint(joint->btJoint);
                delete joint->btJoint;
                joint->btJoint = 0;
            }
        }

        world->removeRigidBody(body);
        delete body;
        body = 0;
    }

    if (ghost_body != 0) {
        world->removeCollisionObject(ghost_body);
        delete ghost_body;
        ghost_body = 0;
    }

    if (shape != 0) { delete shape; shape = 0; }
    if (motionState != 0) { delete motionState; motionState = 0; }

    if (!physicalized) return;

    motionState = new btDefaultMotionState(fromVRTransform( vr_obj, scale ));

    if (physicsShape == "Box") shape = getBoxShape();
    if (physicsShape == "Sphere") shape = getSphereShape();
    if (physicsShape == "Convex") shape = getConvexShape();
    if (physicsShape == "Concave") shape = getConcaveShape();
    if (shape == 0) return;

    btVector3 inertiaVector(0,0,0);
    float _mass = mass;
    if (!dynamic) _mass = 0;

    if (_mass != 0) shape->calculateLocalInertia(_mass, inertiaVector);

    btRigidBody::btRigidBodyConstructionInfo rbInfo( _mass, motionState, shape, inertiaVector );
    if (ghost) {
        ghost_body = new btPairCachingGhostObject();
        ghost_body->setCollisionShape( shape );
        ghost_body->setUserPointer( vr_obj );
        ghost_body->setCollisionFlags( btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE );
        world->addCollisionObject(ghost_body, collisionGroup, collisionMask);
    } else {
        body = new btRigidBody(rbInfo);
        body->setActivationState(activation_mode);
        world->addRigidBody(body, collisionGroup, collisionMask);
    }

    scene->physicalize(vr_obj);
    updateConstraints();
}


btCollisionShape* VRPhysics::getBoxShape() {
    if (shape_param > 0) return new btBoxShape( btVector3(shape_param, shape_param, shape_param) );
    float x,y,z;
    x=y=z=0;

    vector<OSG::VRObject*> geos = vr_obj->getObjectListByType("Geometry");
    for (uint j=0; j<geos.size(); j++) {
        OSG::VRGeometry* geo = (OSG::VRGeometry*)geos[j];
        if (geo == 0) continue;
        OSG::GeoVectorPropertyRecPtr pos = geo->getMesh()->getPositions();
        for (uint i=0;i<pos->size();i++) {
            OSG::Pnt3f p;
            pos->getValue(p,i);
            x = max(x, p[0]*scale[0]);
            y = max(y, p[1]*scale[1]);
            z = max(z, p[2]*scale[2]);
        }
    }

    //cout << "\nConstruct Box shape for " << vr_obj->getName() << ": " << 2*x << " " << 2*y << " " << 2*z << endl;
    return new btBoxShape(btVector3(2*x,2*y,2*z));
}

btCollisionShape* VRPhysics::getSphereShape() {
    if (shape_param > 0) return new btSphereShape( shape_param );

    float r2 = 0;
    int N = 0;
    OSG::Pnt3f p;
    OSG::Pnt3f center;

    auto geos = vr_obj->getObjectListByType("Geometry");

    /*for (auto _g : geos ) { // get geometric center // makes no sense as you would have to change the center of the shape..
        OSG::VRGeometry* geo = (OSG::VRGeometry*)_g;
        OSG::GeoVectorPropertyRecPtr pos = geo->getMesh()->getPositions();
        N += pos->size();
        for (uint i=0; i<pos->size(); i++) {
            pos->getValue(p,i);
            center += p;
        }
    }

    center *= 1.0/N;*/

    for (auto _g : geos ) {
        OSG::VRGeometry* geo = (OSG::VRGeometry*)_g;
        OSG::GeoVectorPropertyRecPtr pos = geo->getMesh()->getPositions();
        for (uint i=0; i<pos->size(); i++) {
            pos->getValue(p,i);
            //r2 = max( r2, (p-center).squareLength() );
            for (int i=0; i<3; i++) p[i] *= scale[i];
            r2 = max( r2, p[0]*p[0]+p[1]*p[1]+p[2]*p[2] );
        }
    }

    return new btSphereShape(sqrt(r2));
}

btCollisionShape* VRPhysics::getConvexShape() {
    btConvexHullShape* shape = new btConvexHullShape();

    vector<OSG::VRObject*> geos = vr_obj->getObjectListByType("Geometry");
    for (uint j=0; j<geos.size(); j++) {
        OSG::VRGeometry* geo = (OSG::VRGeometry*)geos[j];
        if (geo == 0) continue;
        if (geo->getMesh() == 0) continue;
        OSG::GeoVectorPropertyRecPtr pos = geo->getMesh()->getPositions();
        if (pos == 0) continue;
        for (uint i=0;i<pos->size();i++) {
            OSG::Pnt3f p;
            pos->getValue(p,i);
            for (int i=0; i<3; i++) p[i] *= scale[i];
            shape->addPoint(btVector3(p[0], p[1], p[2]));
        }
    }

    shape->setMargin(collisionMargin);

    //cout << "\nConstruct Convex shape for " << vr_obj->getName() << endl;
    return shape;
}

btCollisionShape* VRPhysics::getConcaveShape() {
    btTriangleMesh* tri_mesh = new btTriangleMesh();

    vector<OSG::VRObject*> geos = vr_obj->getObjectListByType("Geometry");
    int N = 0;
    for (uint j=0; j<geos.size(); j++) {
        OSG::VRGeometry* geo = (OSG::VRGeometry*)geos[j];
        if (geo == 0) continue;
        if (geo->getMesh() == 0) continue;
        OSG::TriangleIterator ti(geo->getMesh());

        btVector3 vertexPos[3];

        while(!ti.isAtEnd()) {
            for (int i=0;i<3;i++) {
                OSG::Pnt3f p = ti.getPosition(i);
                for (int j=0;j<3;j++) vertexPos[i][j] = p[j];
            }

            tri_mesh->addTriangle(vertexPos[0]*scale[0], vertexPos[1]*scale[1], vertexPos[2]*scale[2]);
            ++ti;
            N++;
        }
    }
    if (N == 0) return 0;

    //cout << "\nConstruct Concave shape for " << vr_obj->getName() << endl;
    btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(tri_mesh, true);
    return shape;
}

btTransform VRPhysics::fromVRTransform(OSG::VRTransform* t, OSG::Vec3f& scale) {
    OSG::Matrix m = t->getWorldMatrix();

    scale;
    for (int i=0; i<3; i++) scale[i] = m[i].length(); // store scale
    for (int i=0; i<3; i++) m[i] *= 1.0/scale[i]; // normalize

    btTransform bltTrans;
    bltTrans.setFromOpenGLMatrix(&m[0][0]);
    return bltTrans;
}

OSG::Matrix VRPhysics::fromBTTransform(const btTransform t, OSG::Vec3f scale) {
    OSG::Matrix m = fromBTTransform(t);

    // apply scale
    OSG::Matrix s;
    s.setScale(scale);
    m.mult(s);
    return m;
}

OSG::Matrix VRPhysics::fromBTTransform(const btTransform t) {
    btScalar _m[16];
    t.getOpenGLMatrix(_m);

    OSG::Matrix m;
    for (int i=0;i<4;i++) m[0][i] = _m[i];
    for (int i=0;i<4;i++) m[1][i] = _m[4+i];
    for (int i=0;i<4;i++) m[2][i] = _m[8+i];
    for (int i=0;i<4;i++) m[3][i] = _m[12+i];

    //cout << "fromTransform " << m << endl;
    return m;
}

void VRPhysics::pause(bool b) {
    return;
    if (body == 0) return;
    if (dynamic == !b) return;
    setDynamic(!b);
}

void VRPhysics::resetForces() {
    if (body == 0) return;
    body->setAngularVelocity(btVector3(0,0,0));
    body->setLinearVelocity(btVector3(0,0,0));
    body->clearForces();
}

void VRPhysics::applyImpulse(OSG::Vec3f i) {
    if (body == 0) return;
    if (mass == 0) return;
    body->setLinearVelocity(btVector3(i[0]/mass, i[1]/mass, i[2]/mass));
}

void VRPhysics::updateTransformation(OSG::VRTransform* t) {
    if (body) {
        body->setWorldTransform(fromVRTransform(t, scale));
        body->activate();
    }

    if (ghost_body) {
        ghost_body->setWorldTransform(fromVRTransform(t, scale));
        ghost_body->activate();
    }
}

OSG::Matrix VRPhysics::getTransformation() {
    if (body == 0) return OSG::Matrix();
    btTransform t;
    if (body->getMotionState() == 0) return OSG::Matrix();
    body->getMotionState()->getWorldTransform(t);
    return fromBTTransform(t, scale);
}

void VRPhysics::setTransformation(btTransform t) {
    if (body == 0) return;
    body->setWorldTransform(t);
}

void VRPhysics::setConstraint(VRPhysics* p, OSG::VRConstraint* c, OSG::VRConstraint* cs) {
    if (body == 0) return;
    if (p->body == 0) return;

    if (joints.count(p) == 0) joints[p] = new VRPhysicsJoint(p, c, cs);
    else {
        joints[p]->constraint = c;
        joints[p]->spring = cs;
    }
    if (p->joints2.count(this) == 0) p->joints2[this] = joints[p];
    updateConstraint(p);
}

void VRPhysics::updateConstraint(VRPhysics* p) {
    if (body == 0) return;
    if (p->body == 0) return;
    if (joints.count(p) == 0) return;

    VRPhysicsJoint* joint = joints[p];
    OSG::VRConstraint* c = joint->constraint;
    if (c == 0) return;

    if (joint->btJoint != 0) {
        world->removeConstraint(joint->btJoint);
        delete joint->btJoint;
        joint->btJoint = 0;
    }

    // TODO: possible bug - p is not valid, may have been deleted!

    //cout << "\nCreate Joint " << fromTransform(body->getWorldTransform())[3] << " " << fromTransform(p->body->getWorldTransform())[3] << endl;
    btTransform t = p->body->getWorldTransform().inverse();
    t.mult(t, body->getWorldTransform()); // the position of the first object in the local coords of the second
    joint->btJoint = new btGeneric6DofSpringConstraint(*body, *p->body, btTransform::getIdentity(), t, true);
    world->addConstraint(joint->btJoint, true);

    for (int i=0; i<6; i++) {
        joint->btJoint->setParam(BT_CONSTRAINT_STOP_CFM, 0, i);
        joint->btJoint->setParam(BT_CONSTRAINT_STOP_ERP, 0.6, i);
    }

    joint->btJoint->setLinearLowerLimit(btVector3(c->getMin(0), c->getMin(1), c->getMin(2)));
    joint->btJoint->setLinearUpperLimit(btVector3(c->getMax(0), c->getMax(1), c->getMax(2)));
    joint->btJoint->setAngularLowerLimit(btVector3(c->getMin(3), c->getMin(4), c->getMin(5)));
    joint->btJoint->setAngularUpperLimit(btVector3(c->getMax(3), c->getMax(4), c->getMax(5)));


    // SPRING PARAMETERS

    OSG::VRConstraint* cs = joint->spring;
    if (cs == 0) return;
    for (int i=0; i<6; i++) {
        bool b = (cs->getMin(i) > 0);
        float stiffness = cs->getMin(i);
        float damping = cs->getMax(i);
        joint->btJoint->enableSpring(i, b);
        joint->btJoint->setStiffness(i, stiffness);
        joint->btJoint->setDamping(i, damping);
        joint->btJoint->setEquilibriumPoint(i);
    }
}

void VRPhysics::updateConstraints() {
    if (body == 0) return;
    for (auto j : joints) updateConstraint(j.first);
    for (auto j : joints2) j.first->updateConstraint(this);
}
