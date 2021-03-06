#include "VRPyWeather.h"

#include "core/scripting/VRPyBaseT.h"
#include "core/scripting/VRPyBaseFactory.h"

using namespace OSG;

simpleVRPyType(Rain, New_ptr);
simpleVRPyType(RainCarWindshield, New_ptr);

PyMethodDef VRPyRain::methods[] = {
    {"start", PyWrap(Rain, start, "Starts Rain", void) },
    {"stop", PyWrap(Rain, stop, "Stops Rain", void) },
    {"setScale", PyWrap(Rain, setScale, "Sets Scale, live change: changes weather live (True or False)", void, bool, float) },
    {"get", PyWrap(Rain, get, "Gets Scale", float ) },
    //{"overrideParameters", PyWrap(Rain, overrideParameters, "7 parameters: tTransition, densRain, densCL, vX, vY, clCL, light \n\t", void, float, float, float, float, float, float, float ) },
    //{"doTestFunction", PyWrap(Rain, doTestFunction, "doTestFunction", void) },
    {"getRenderer", PyWrap(Rain, getRenderer, "Get renderer", VRTextureRendererPtr) },
    //{"getTexMat", PyWrap(Rain, getTexMat, "getTexMat", VRMaterialPtr) },
    {"setDropColor", PyWrap(Rain, setDropColor, "Set drop color", void, Vec3d) },
    {"setDropSize", PyWrap(Rain, setDropSize, "setDropSize - width, length", void, float, float) },
    {"setDropSpeed", PyWrap(Rain, setDropSpeed, "setDropSpeed", void, float) },
    {"setDropDensity", PyWrap(Rain, setDropDensity, "setDropDensity", void, float) },
    {NULL}  /* Sentinel */
};

PyMethodDef VRPyRainCarWindshield::methods[] = {
    //{"doTestFunction", PyWrap(RainCarWindshield, doTestFunction, "do test function", void ,void) },
    {"setWindshield", PyWrap(RainCarWindshield, setWindshield, "setWindshield", void, VRGeometryPtr) },
    {"setScale", PyWrap(RainCarWindshield, setScale, "sets scale", void, bool, float) },
    {"setWipers", PyWrap(RainCarWindshield, setWipers, "setWipers", void, bool, float) },
    {"start", PyWrap(RainCarWindshield, start, "starts rain on windshield", void, void) },
    {"stop", PyWrap(RainCarWindshield, stop, "stops rain on windshield", void, void) },
    {"setWiperPos", PyWrap(RainCarWindshield, setWiperPos, "reposition wipers on windshield", void, Vec2d) },
    {"setWiperSize", PyWrap(RainCarWindshield, setWiperSize, "sets wiper length", void, float) },
    {"cutPower", PyWrap(RainCarWindshield, cutPower, "cuts power, wipers will stop where they are", void, void) },
    {NULL}  /* Sentinel */
};

