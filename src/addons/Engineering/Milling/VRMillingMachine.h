#ifndef VRMILLINGMACHINE_H_INCLUDED
#define VRMILLINGMACHINE_H_INCLUDED

#include <string>
#include <vector>
#include <OpenSG/OSGConfig.h>
#include "core/math/OSGMathFwd.h"
#include "core/objects/VRObjectFwd.h"

OSG_BEGIN_NAMESPACE;
using namespace std;

class VRSocket;

class VRMillingMachine {
    private:
        int axis = 3;
        vector<VRTransformPtr> geos;

        VRSocket* http = 0;

        string address;
        bool online = false;
        int state = 1; // CNC Status (z.B. Achsen aktiv)
        int mode = 1; // CNC Modus (z.B. JOG, Auto, ..)

        Vec3d* pos = 0;
        float speed = 4;

        string post(string cmd, string data);

    public:
        VRMillingMachine();
        ~VRMillingMachine();
        static shared_ptr<VRMillingMachine> create();

        void connect(string s);
        void disconnect();
        bool connected();

        void setSpeed(Vec3d v);
        void setSpeed(float s);
        void setGeometry(vector<VRTransformPtr> geos);

        void setPosition(Vec3d p);
        Vec3d getPosition();

        int getState();
        int getMode();

        void update();
};

typedef shared_ptr<VRMillingMachine> VRMillingMachinePtr;

OSG_END_NAMESPACE;

#endif // VRMILLINGMACHINE_H_INCLUDED
