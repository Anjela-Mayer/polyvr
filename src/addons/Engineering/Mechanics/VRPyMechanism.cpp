#include "VRPyMechanism.h"
#include "core/scripting/VRPyBaseT.h"
#include "core/scripting/VRPyGeometry.h"
#include "core/scripting/VRPyTypeCaster.h"

using namespace OSG;

simpleVRPyType(Mechanism, New_ptr);

PyMethodDef VRPyMechanism::methods[] = {
    {"add", (PyCFunction)VRPyMechanism::add, METH_VARARGS, "Add part to mechanism - add(P)" },
    {"update", (PyCFunction)VRPyMechanism::update, METH_NOARGS, "Update mechanism simulation" },
    {"clear", (PyCFunction)VRPyMechanism::clear, METH_NOARGS, "Clear mechanism parts" },
    {"addChain", (PyCFunction)VRPyMechanism::addChain, METH_VARARGS, "Add chain - addChain(float width, [G1, G2, G3, ...])" },
    {"addGear", PyWrap(Mechanism, addGear, "Add custom geo as gear, (geo, width, hole, pitch, N_teeth, teeth_size, bevel)", void, VRTransformPtr, float, float, float, int, float, float) },
    {NULL}  /* Sentinel */
};

PyObject* VRPyMechanism::add(VRPyMechanism* self, PyObject* args) {
    if (!self->valid()) return NULL;

    VRPyGeometry* geo;
    if (! PyArg_ParseTuple(args, "O", &geo)) return NULL;

    self->objPtr->add(geo->objPtr);
    Py_RETURN_TRUE;
}

PyObject* VRPyMechanism::update(VRPyMechanism* self) {
    if (!self->valid()) return NULL;
    self->objPtr->update();
    Py_RETURN_TRUE;
}

PyObject* VRPyMechanism::clear(VRPyMechanism* self) {
    if (!self->valid()) return NULL;
    self->objPtr->clear();
    Py_RETURN_TRUE;
}

PyObject* VRPyMechanism::addChain(VRPyMechanism* self, PyObject* args) {
    if (!self->valid()) return NULL;
    float w; PyObject *l, *dirs;
    if (! PyArg_ParseTuple(args, "fOO", &w, &l, &dirs)) return NULL;
    vector<PyObject*> objs = pyListToVector(l);
    vector<OSG::VRTransformPtr> geos;
    for (auto o : objs) {
        VRPyTransform* g = (VRPyTransform*)o;
        geos.push_back( g->objPtr );
    }
    return VRPyTypeCaster::cast( self->objPtr->addChain(w, geos, PyString_AsString(dirs) ) );
}
