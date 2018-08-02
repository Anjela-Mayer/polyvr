#include "VRPyDevice.h"
#include "VRPyMobile.h"
#include "VRPyMouse.h"
#include "VRPyMultiTouch.h"
#include "VRPyHaptic.h"
#include "addons/LeapMotion/VRPyLeap.h"
#include "VRPyTransform.h"
#include "VRPyGeometry.h"
#include "VRPySprite.h"
#include "core/objects/VRTransform.h"
#include "VRPyBaseT.h"
#include "VRPyTypeCaster.h"

using namespace OSG;

simpleVRPyType(Signal, 0)
simpleVRPyType(Device, New_named_ptr)

PyMethodDef VRPySignal::methods[] = {
    {NULL}  /* Sentinel */
};

PyMethodDef VRPyDevice::methods[] = {
    {"getBeacon", PyWrapOpt( Device, getBeacon, "Get device beacon. - DeviceBeacon getBeacon(int beaconId = 0)", "0", VRTransformPtr, int ) },
    {"setBeacon", PyWrapOpt( Device, setBeacon, "Set device beacon.", "0", void, VRTransformPtr, int ) },
    {"getTarget", PyWrap( Device, getTarget, "Get device target.", VRTransformPtr ) },
    {"setTarget", PyWrap( Device, setTarget, "Set device target.", void, VRTransformPtr ) },
    {"getKey", PyWrap( Device, key, "Get activated device key.", int ) },
    {"getState", PyWrap( Device, getState, "Get device state.", int ) },
    {"getKeyState", PyWrap( Device, b_state, "Get device key state.", int, int ) },
    {"getSlider", PyWrap( Device, s_state, "Get device slider state.", float, int ) },
    {"getMessage", PyWrap( Device, getMessage, "Get device received message.", string ) },
    {"getType", PyWrap(Device, getType, "Get device type.", string ) },
    {"setDnD", PyWrap( Device, setDnD, "Set drag && drop.", void, bool ) },
    {"intersect", PyWrapOpt(Device, intersect2, "Attempts to intersect the device beacon with the scene - \n"
                                                                    "bool intersect(Object scene, bool force, DeviceBeacon beacon, Vec3 dir)\n\n"
                                                                    "  return: True, if intersection successful, otherwise False\n\n"
                                                                    "  scene:  [optional] default=VR.Scene()\n"
                                                                    "          Specifies object in scene graph which is checked incl. its children.\n\n"
                                                                    "  force:  [optional] default=False\n"
                                                                    "          Forces reevaluation of intersect, if False existing intersection from same frame can be used.\n\n"
                                                                    "  beacon: [optional] default=device.getBeacon()\n"
                                                                    "          Specifies which beacon of the device will be intersected, in case of multiple beacons (Multitouch).\n\n"
                                                                    "  dir:    [optional] default=[0,0,-1]\n"
                                                                    "          Currently not implemented! Creates a beacon from device position in given direction for intersect.", "0|0|0|0 0 -1", bool, VRObjectPtr, bool, VRTransformPtr, Vec3d ) },
    {"getIntersected", PyWrap(Device, getIntersected, "Get device intersected object.", VRObjectPtr ) },
    {"getIntersection", PyWrap(Device, getIntersectionPoint, "Get device intersection point", Pnt3d ) },
    {"getIntersectionNormal", PyWrap(Device, getIntersectionNormal, "Get normal at intersection point", Vec3d ) },
    {"getIntersectionUV", PyWrap(Device, getIntersectionUV, "Get uv at intersection point", Vec2d ) },
    {"getIntersectionTriangle", PyWrap(Device, getIntersectionTriangle, "Get triangle at intersection point", Vec3i ) },
    {"addIntersection", PyWrap( Device, addIntersection, "Add device intersection node.", void, VRObjectPtr ) },
    {"remIntersection", PyWrap( Device, remIntersection, "Remove device intersection node.", void, VRObjectPtr ) },
    {"getDragged", PyWrap( Device, getDragged, "Get dragged object.", VRTransformPtr ) },
    {"getDragGhost", PyWrap( Device, getDragGhost, "Get drag ghost.", VRTransformPtr ) },
    {"drag", PyWrap( Device, drag, "Start to drag an object", void, VRObjectPtr ) },
    {"drop", PyWrap( Device, drop, "Drop any object", void ) },
    {"setSpeed", PyWrap( Device, setSpeed, "Set the navigation speed of the device", void, Vec2d ) },
    {"getSpeed", PyWrap( Device, getSpeed, "Get the navigation speed of the device", Vec2d ) },
    {"addSignal", PyWrap( Device, newSignal, "Add a new signal, key, state", VRSignalPtr, int, int ) },
    {"trigger", PyWrap( Device, change_button, "Trigger signal, key, state", void, int, int ) },
    {NULL}  /* Sentinel */
};

PyObject* VRPyDevice::fromSharedPtr(VRDevicePtr dev) {
    string type = dev->getType();
    if (type == "mouse") return VRPyMouse::fromSharedPtr( static_pointer_cast<VRMouse>(dev) );
    else if (type == "multitouch") return VRPyMultiTouch::fromSharedPtr( static_pointer_cast<VRMultiTouch>(dev) );
    else if (type == "leap") return VRPyLeap::fromSharedPtr( static_pointer_cast<VRLeap>(dev) );
    else if (type == "server") return VRPyServer::fromSharedPtr( static_pointer_cast<VRServer>(dev) );
    else if (type == "haptic") return VRPyHaptic::fromSharedPtr( static_pointer_cast<VRHaptic>(dev) );
    else if (type == "keyboard") return VRPyBaseT<VRDevice>::fromSharedPtr( dev );
    else if (type == "flystick") return VRPyBaseT<VRDevice>::fromSharedPtr( dev );
    cout << "\nERROR in VRPyTypeCaster::cast device: " << type << " not handled!\n";
    return VRPyBaseT<VRDevice>::fromSharedPtr(dev);
}
/*
<<<<<<< HEAD
PyObject* VRPyDevice::addSignal(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::setSpeed, Object is invalid"); return NULL; }
    int k, s;
    if (! PyArg_ParseTuple(args, "ii", &k, &s)) return NULL;
    self->objPtr->addSignal( k,s );
    Py_RETURN_TRUE;
}

PyObject* VRPyDevice::trigger(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::setSpeed, Object is invalid"); return NULL; }
    int k, s;
    if (! PyArg_ParseTuple(args, "ii", &k, &s)) return NULL;
    self->objPtr->change_button( k,s );
    Py_RETURN_TRUE;
}

PyObject* VRPyDevice::setSpeed(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::setSpeed, Object is invalid"); return NULL; }
    self->objPtr->setSpeed( parseVec2f(args) );
    Py_RETURN_TRUE;
}

PyObject* VRPyDevice::intersect(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::intersect, Object is invalid"); return NULL; }

    VRPyObject* o = 0;          // root of sub scene graph to be intersected with
    VRPyTransform* c = 0;       // caster, beacon to be used for intersection
    PyObject* d = 0;            // additional direction vector, currently unused
    int force = 0;
    if (! PyArg_ParseTuple(args, "|OiOO", &o, &force, &c, &d)) return NULL;
    OSG::VRIntersection ins = self->objPtr->intersect(o ? o->objPtr : 0, force, c ? c->objPtr : 0, d ? parseVec3dList(d) : Vec3d(0,0,-1));
    if (ins.hit) Py_RETURN_TRUE;
    else Py_RETURN_FALSE;
}

PyObject* VRPyDevice::drag(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::drag, Object is invalid"); return NULL; }
    OSG::VRObjectPtr obj = 0;
    if (!VRPyObject::parse(args, &obj)) return NULL;
    string name = obj->getName();
    self->objPtr->drag(obj, self->objPtr->getBeacon());
    Py_RETURN_TRUE;
}

PyObject* VRPyDevice::drop(VRPyDevice* self) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::drop, Object is invalid"); return NULL; }
    self->objPtr->drop();
    Py_RETURN_TRUE;
}

PyObject* VRPyDevice::getName(VRPyDevice* self) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getName, Object is invalid"); return NULL; }
    return PyString_FromString(self->objPtr->getName().c_str());
}

PyObject* VRPyDevice::destroy(VRPyDevice* self) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::destroy, Object is invalid"); return NULL; }
    self->objPtr = 0;
    Py_RETURN_TRUE;
}

PyObject* VRPyDevice::getBeacon(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getBeacon, Object is invalid"); return NULL; }
    int beaconId = 0;
    if (! PyArg_ParseTuple(args, "|i", &beaconId)) return NULL;
    return VRPyTransform::fromSharedPtr(self->objPtr->getBeacon(beaconId));
}

PyObject* VRPyDevice::setBeacon(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::setBeacon, Object is invalid"); return NULL; }
    VRPyTransform* beacon = NULL;
    int beaconId = 0;
    if (! PyArg_ParseTuple(args, "O|i", &beacon, &beaconId)) return NULL;
    self->objPtr->setBeacon(beacon->objPtr, beaconId);
    Py_RETURN_TRUE;
}

PyObject* VRPyDevice::getTarget(VRPyDevice* self) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getTarget, Object is invalid"); return NULL; }
    return VRPyTransform::fromSharedPtr(self->objPtr->getTarget());
}

PyObject* VRPyDevice::setTarget(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::setTarget, Object is invalid"); return NULL; }
    VRPyTransform* target = NULL;
    if (! PyArg_ParseTuple(args, "O", &target)) return NULL;
    self->objPtr->setTarget(target->objPtr);
    Py_RETURN_TRUE;
}

PyObject* VRPyDevice::getState(VRPyDevice* self) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getState, Object is invalid"); return NULL; }
    return PyInt_FromLong(self->objPtr->getState());
}

PyObject* VRPyDevice::getKeyState(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getKeyState, Object is invalid"); return NULL; }
    int i=0;
    if (! PyArg_ParseTuple(args, "i", &i)) return NULL;
    return PyInt_FromLong(self->objPtr->b_state(i));
}

PyObject* VRPyDevice::getKey(VRPyDevice* self) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getKey, Object is invalid"); return NULL; }
    return PyInt_FromLong(self->objPtr->key());
}

PyObject* VRPyDevice::getMessage(VRPyDevice* self) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getKey, Object is invalid"); return NULL; }
    return PyString_FromString(self->objPtr->getMessage().c_str());
}

PyObject* VRPyDevice::getSlider(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getSlider, Object is invalid"); return NULL; }
    int i=0;
    if (! PyArg_ParseTuple(args, "i", &i)) return NULL;
    return PyFloat_FromDouble(self->objPtr->s_state(i));
}

PyObject* VRPyDevice::getType(VRPyDevice* self) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getType, Object is invalid"); return NULL; }
    return PyString_FromString(self->objPtr->getType().c_str());
}

PyObject* VRPyDevice::setDnD(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::setDnD, Object is invalid"); return NULL; }
    int i=0;
    if (! PyArg_ParseTuple(args, "i", &i)) return NULL;
    self->objPtr->toggleDragnDrop(i);
    Py_RETURN_TRUE;
}

PyObject* VRPyDevice::getIntersected(VRPyDevice* self) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getIntersected, Object is invalid"); return NULL; }
    return VRPyTypeCaster::cast(self->objPtr->getLastIntersection().object.lock());
}

PyObject* VRPyDevice::addIntersection(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::addIntersection, Object is invalid"); return NULL; }
    VRPyObject* iobj = NULL;
    if (! PyArg_ParseTuple(args, "O", &iobj)) return NULL;
    self->objPtr->addDynTree(iobj->objPtr);
    Py_RETURN_TRUE;
}

PyObject* VRPyDevice::remIntersection(VRPyDevice* self, PyObject *args) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::remIntersection, Object is invalid"); return NULL; }
    VRPyObject* iobj = NULL;
    if (! PyArg_ParseTuple(args, "O", &iobj)) return NULL;
    self->objPtr->remDynTree(iobj->objPtr);
    Py_RETURN_TRUE;
}

PyObject* VRPyDevice::getDragGhost(VRPyDevice* self) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getDragGhost, Object is invalid"); return NULL; }
    return VRPyTypeCaster::cast(self->objPtr->getDraggedGhost());
}

PyObject* VRPyDevice::getDragged(VRPyDevice* self) {
    if (self->objPtr == 0) { PyErr_SetString(err, "VRPyDevice::getDragged, Object is invalid"); return NULL; }
    return VRPyTypeCaster::cast(self->objPtr->getDraggedObject());
}

=======
>>>>>>> upstream/master*/
