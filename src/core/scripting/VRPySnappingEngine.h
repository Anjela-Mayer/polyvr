#ifndef VRPYSNAPPINGENGINE_H_INCLUDED
#define VRPYSNAPPINGENGINE_H_INCLUDED

#include "VRPyBase.h"
#include "core/tools/VRSnappingEngine.h"

struct VRPySnappingEngine : VRPyBaseT<OSG::VRSnappingEngine> {
    static PyMethodDef methods[];
};

#endif // VRPYSNAPPINGENGINE_H_INCLUDED
