#include "VRMultiWindow.h"
#include "VRView.h"
#include <OpenSG/OSGRemoteAspect.h>
#include <OpenSG/OSGFieldContainerFactory.h>
#include <OpenSG/OSGNameAttachment.h>
#include "core/utils/toString.h"
#include <libxml++/nodes/element.h>
#include "core/scene/VRSceneManager.h"
#include "core/utils/VRFunction.h"


#include <OpenSG/OSGChangeList.h>
#include <OpenSG/OSGThread.h>
#include <OpenSG/OSGThreadManager.h>
#include <OpenSG/OSGBarrier.h>

OSG_BEGIN_NAMESPACE;
using namespace std;


VRMultiWindow::VRMultiWindow() {
    type = 0;
}

VRMultiWindow::~VRMultiWindow() {
    win = 0;
}

VRMultiWindowPtr VRMultiWindow::ptr() { return static_pointer_cast<VRMultiWindow>( shared_from_this() ); }
VRMultiWindowPtr VRMultiWindow::create() { return shared_ptr<VRMultiWindow>(new VRMultiWindow() ); }

void VRMultiWindow::addServer(string server) { servers.push_back(server); }

string VRMultiWindow::getServer(int x, int y) {
    uint i = x+y*Nx;
    if (i >= servers.size()) return "None";
    return servers[i];
}


void VRMultiWindow::setServer(int x, int y, string s) {
    uint i = x+y*Nx;
    while (i >= servers.size()) servers.push_back("");
    servers[i] = s;
    cout << "\nWM SERVER SET\n";
}

void VRMultiWindow::setNTiles(int x, int y) {
    Nx = x;
    Ny = y;
    int N = Nx*Ny;
    servers.resize(N);
}

int VRMultiWindow::getNXTiles() { return Nx; }
int VRMultiWindow::getNYTiles() { return Ny; }

bool VRMultiWindow::init_win(const std::string &msg, const std::string &server, Real32 progress) {
    cout << endl << msg << " to " << server << " : " << progress;
    if (progress == 1) { state = JUSTCONNECTED; return true; }
    if (tries == 3) { state = NO_CONNECTION; return false; }
    tries++;
    return true;
}

void VRMultiWindow::initialize() {
    cout << "Initializing MultiWindow\n";
    //cout << " Render MW " << getName() << " state " << getStateString() << endl;
    win = 0; _win = 0; tries = 0; state = CONNECTING;
    win = MultiDisplayWindow::create(); _win = win;

    win->setSize(width, height);
    win->setHServers(Nx);
    win->setVServers(Ny);

    //win->setConnectionType(connection_type); // "Multicast", "SockPipeline" // not needed apparently!
    for (auto s : servers) win->editMFServers()->push_back(s);
    for (auto wv : views) if (auto v = wv.lock()) v->setWindow(win);

    ClusterWindow::ConnectionCB cb = boost::bind(&VRMultiWindow::init_win, this, _1, _2, _3);
    win->initAsync(cb);
    //cout << endl << " render once " << endl;
    //if (state == CONNECTED) win->render(ract);
    cout << " done " << getStateString() << endl;
}

/**

clustering issues may arise when not calling _win->render() after commiting changelist changes!
keep this in mind when trying to optimize regarding to system state like the flags 'content' and 'active'

*/

void VRMultiWindow::render(bool fromThread) {
    //if (state == INITIALIZING) { initialize(); return; }

    if (state == JUSTCONNECTED) {
        Thread::getCurrentChangeList()->clear();
        Thread::getCurrentChangeList()->fillFromCurrentState();
        state = CONNECTED;
    }

    if (state == CONNECTED) {
        try { _win->render(ract); }
        catch(exception& e) { reset(); }
    }
}

void VRMultiWindow::reset() { state = INITIALIZING; }
int VRMultiWindow::getState() { return state; }
void VRMultiWindow::setConnectionType(string ct) { connection_type = ct; }
string VRMultiWindow::getConnectionType() { return connection_type; }

string VRMultiWindow::getStateString() {
    if (state == CONNECTED) return "connected";
    if (state == INITIALIZING) return "initializing";
    if (state == CONNECTING) return "connecting";
    if (state == NO_CONNECTION) return "not connencted";
    return "invalid state";
}

void VRMultiWindow::save(xmlpp::Element* node) {
    VRWindow::save(node);
    node->set_attribute("Nx", toString(Nx));
    node->set_attribute("Ny", toString(Ny));
    node->set_attribute("ConnType", connection_type);

    xmlpp::Element* sn;
    for (uint i=0; i<servers.size(); i++) {
        string s = servers[i];
        sn = node->add_child("Server");
        sn->set_attribute("Address", s);
    }
}

void VRMultiWindow::load(xmlpp::Element* node) {
    VRWindow::load(node);
    Nx = toInt( node->get_attribute("Nx")->get_value() );
    Ny = toInt( node->get_attribute("Ny")->get_value() );
    if (node->get_attribute("ConnType")) connection_type = node->get_attribute("ConnType")->get_value();

    xmlpp::Node::NodeList nl = node->get_children();
    xmlpp::Node::NodeList::iterator itr;
    for (itr = nl.begin(); itr != nl.end(); itr++) {
        xmlpp::Node* n = *itr;

        xmlpp::Element* el = dynamic_cast<xmlpp::Element*>(n);
        if (!el) continue;

        string name = el->get_name();
        if (name != "Server") continue;

        string addr = el->get_attribute("Address")->get_value();

        addServer(addr);
    }

    setNTiles(Nx, Ny);
}

OSG_END_NAMESPACE;
