#ifndef VRSyncNode_H_INCLUDED
#define VRSyncNode_H_INCLUDED

#include "VRTransform.h"
#include "core/networking/VRNetworkingFwd.h"
#include <OpenSG/OSGChangeList.h>

OSG_BEGIN_NAMESPACE;
using namespace std;

ptrFwd(VRSyncRemote);

class VRSyncRemote {//: public VRName {
    private:
        map<int, int> mapping; // <remote container ID, local container ID>
        string uri;

        VRWebSocketPtr socket;

    public:
        VRSyncRemote(string uri = "");
        ~VRSyncRemote();

        void connect();
        bool send(string message);
        static VRSyncRemotePtr create(string name = "None");
//        VRSyncRemotePtr ptr();
};

class VRSyncNode : public VRTransform {
    private:
        VRSocketPtr socket;
        VRFunction<void*>* socketCb;
        VRUpdateCbPtr updateFkt;

        map<int, bool> container; // local containers
        map<string, VRSyncRemotePtr> remotes;

        VRObjectPtr copy(vector<VRObjectPtr> children);

        void update();
        void handleChangeList(void* msg);
        vector<FieldContainer*> findContainer(string typeName);
        vector<FieldContainer*> getTransformationContainer(ChangeList* cl);
        //vector<OSG::Field

    public:
        VRSyncNode(string name = "syncNode");
        ~VRSyncNode();

        static VRSyncNodePtr create(string name = "None");
        VRSyncNodePtr ptr();

        void startInterface(int port);

        void addRemote(string host, int port, string name);

        void printChangeList();
        void broadcast(string message);

        void getContainer();
        //void getContainerFields();

        string printContainer(vector<FieldContainer*> container);

};

OSG_END_NAMESPACE;

#endif // VRSyncNode_H_INCLUDED