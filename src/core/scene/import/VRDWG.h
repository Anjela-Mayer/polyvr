#ifndef VRDWG_H_INCLUDED
#define VRDWG_H_INCLUDED

#include <OpenSG/OSGConfig.h>
#include <string>
#include <map>
#include "core/objects/VRObjectFwd.h"

OSG_BEGIN_NAMESPACE;
using namespace std;

void loadDWG(string path, VRTransformPtr res, map<string, string> options);
void writeDWG(VRObjectPtr res, string path);
VRGeometryPtr dwgArcTest();

OSG_END_NAMESPACE;

#endif // VRDWG_H_INCLUDED
