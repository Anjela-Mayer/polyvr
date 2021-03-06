#ifndef VRTCPCLIENT_H_INCLUDED
#define VRTCPCLIENT_H_INCLUDED

#include "../VRNetworkingFwd.h"
#include <string>

class TCPClient;

using namespace std;
namespace OSG {

class VRTCPClient {
    private:
        TCPClient* client = 0;

    public:
        VRTCPClient();
        ~VRTCPClient();

        static VRTCPClientPtr create();

        void connect(string host, int port);
        void connect(string uri);
        void send(const string& message);
        bool connected();
};

}

#endif // VRTCPCLIENT_H_INCLUDED
