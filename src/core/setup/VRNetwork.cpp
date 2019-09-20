#include "VRNetwork.h"
#include "core/utils/VRManager.cpp"
#include "core/networking/VRPing.h"
#include "core/networking/VRSSH.h"
#include "core/gui/VRGuiUtils.h"

#include "core/scene/VRSceneManager.h"
#include "core/utils/system/VRSystem.h"

using namespace OSG;

//template<> class VRManager<VRNetworkNode>;
//template<> VRNetworkNodePtr VRManager<VRNetworkNode>::add(string name);

VRNetwork::VRNetwork() : VRManager("Network") {}
VRNetwork::~VRNetwork() {
    cout << "~VRNetwork\n";
    stopSlaves();
}


VRNetworkNode::VRNetworkNode(string name) : VRManager("NetworkNode") {
    setNameSpace("NetworkNode");
    setName(name);

    slavePath = VRSceneManager::get()->getOriginalWorkdir();

    store("address", &address);
    store("user", &user);
    store("slavePath", &slavePath);
    regStorageSetupFkt( VRStorageCb::create("network_node_update", boost::bind(&VRNetworkNode::setup, this, _1)) );
}

VRNetworkNode::~VRNetworkNode() { stopSlaves(); }

VRNetworkNodePtr VRNetworkNode::create(string name) { return VRNetworkNodePtr( new VRNetworkNode(name) ); }
VRNetworkNodePtr VRNetworkNode::ptr() { return static_pointer_cast<VRNetworkNode>( shared_from_this() ); }

void VRNetworkNode::setup(VRStorageContextPtr context) {
    update();
    initSlaves();
}

string VRNetworkNode::getAddress() { return address; }
string VRNetworkNode::getUser() { return user; }
string VRNetworkNode::getSlavePath() { return slavePath; }

void VRNetworkNode::setAddress(string s) { address = s; update(); }
void VRNetworkNode::setUser(string s) { user = s; update(); }
void VRNetworkNode::setSlavePath(string s) { slavePath = s; update(); }

string VRNetworkNode::getStatNode() { return stat_node; }
string VRNetworkNode::getStatSSH() { return stat_ssh; }
string VRNetworkNode::getStatSSHkey() { return stat_ssh_key; }
string VRNetworkNode::getStatPath() { return stat_path; }

void VRNetworkNode::set(string a, string u, string p) {
    address = a;
    user = u;
    slavePath = p;
    update();
}

void VRNetworkNode::distributeKey() {
    if (stat_node != "ok") return;
    auto ssh = VRSSHSession::open(address, user);
    if (!ssh->hasLocalKey()) ssh->createLocalKey();
    ssh->distrib_key();
    stat_ssh = ssh->getStat();
    stat_ssh_key = ssh->getKeyStat();
    ssh.reset();
    update();
}

VRNetworkSlavePtr VRNetworkNode::add(string name) {
    auto s = VRManager<VRNetworkSlave>::add(name);
    s->setNode(ptr());
    return s;
}

void VRNetworkNode::initSlaves() {
    bool hasAutostart = false;
    for (auto s : getData()) if (s->getAutostart()) hasAutostart = true;
    if (hasAutostart) stopSlaves();

    for (auto s : getData()) {
        s->setNode(ptr());
        if (s->getAutostart()) {
            s->start();
        }
    }
    if (hasAutostart) sleep(10);
}

void VRNetwork::stopSlaves() {
    for (auto n : getData()) n->stopSlaves();
}

void VRNetworkNode::stopSlaves() {
    string script = getSlavePath() + "/src/cluster/stop";
    execCmd(script);
    update();
}

bool VRNetworkNode::hasFile(string path) {
    string res = execCmd("ls "+path, true);
    bool b = res.substr(0, path.size()) == path;
    return b;
}

void VRNetworkNode::update() {
    stat_node = "ok";
    stat_ssh = "";
    stat_ssh_key = "";
    stat_path = "";

    VRPing p;
    if ( !p.start(address, 1) ) { stat_node = "not reachable"; return; }
    if ( !p.start(address, "22", 1) ) { stat_node = "no ssh"; return; }

    auto ssh = VRSSHSession::open(address, user);
    stat_ssh = ssh->getStat();
    stat_ssh_key = ssh->getKeyStat();
    if (stat_ssh != "ok") { stat_path = "no ssh access"; return; }

    bool b1 = hasFile(slavePath + "/src/cluster/start");
    bool b2 = hasFile(slavePath + "/src/cluster/stop");
    if (b1 && b2) stat_path = "ok";
    else stat_path = "not found";
}

string VRNetworkNode::execCmd(string cmd, bool read) {
    if (stat_node != "ok" || stat_ssh != "ok") return "";
    auto ssh = VRSSHSession::open(address, user);
    return ssh->exec_cmd(cmd, read);
}

VRNetworkSlave::VRNetworkSlave(string name) {
    setNameSpace("NetworkNode");
    setName(name);

    store("connection_type", &connection_type);
    store("fullscreen", &fullscreen);
    store("active_stereo", &active_stereo);
    store("display", &display);
    store("autostart", &autostart);
    store("port", &port);
}

VRNetworkSlave::~VRNetworkSlave() {}

VRNetworkSlavePtr VRNetworkSlave::create(string name) { return VRNetworkSlavePtr( new VRNetworkSlave(name) ); }

void VRNetworkSlave::start() {
    if (!node) return;
    //if (!exists(path + "VRServer")) { stat = "no slave exec. VRServer in src/cluster/"; return; } // TODO: check on remote!

    string disp = "export DISPLAY=\"" + display + "\" && ";
    string pipes = " > /dev/null 2> /dev/null < /dev/null &";
    string script = node->getSlavePath() + "/src/cluster/start";
    //string pipes = " > /dev/null 2> /dev/null < /dev/null"; // TODO: without & it returns the correct code, but it also makes the app stuck!
    string args;
    if (!fullscreen) args += " -w";
    if (active_stereo) args += " -A";
    if (connection_type == "Multicast") args += " -m " + getName();
    if (connection_type == "SockPipeline") args += " -p " + node->getAddress() + ":" + toString(port);
    if (connection_type == "StreamSock") args += " " + node->getAddress() + ":" + toString(port);

    stat = node->execCmd(disp + script + args + pipes, false);
    update();
}

void VRNetworkSlave::stop() {
    if (!node) return;
    node->stopSlaves();
    update();
}

void VRNetworkSlave::setNode(VRNetworkNodePtr n) { node = n; }

void VRNetworkSlave::set(string ct, bool fs, bool as, bool au, string a, int p) {
    connection_type = ct;
    fullscreen = fs;
    active_stereo = as;
    autostart = au;
    display = a;
    port = p;
    update();
}

void VRNetworkSlave::update() {}

string VRNetworkSlave::getStatMulticast() { return stat_multicast; }
string VRNetworkSlave::getStat() { return stat; }

string VRNetworkSlave::getDisplay() { return display; }
string VRNetworkSlave::getConnectionType() { return connection_type; }
bool VRNetworkSlave::getFullscreen() { return fullscreen; }
bool VRNetworkSlave::getActiveStereo() { return active_stereo; }
bool VRNetworkSlave::getAutostart() { return autostart; }
int VRNetworkSlave::getPort() { return port; }

void VRNetworkSlave::setDisplay(string a) { display = a; update(); }
void VRNetworkSlave::setConnectionType(string b) { connection_type = b; update(); }
void VRNetworkSlave::setFullscreen(bool b) { fullscreen = b; update(); }
void VRNetworkSlave::setAutostart(bool b) { autostart = b; update(); }

string VRNetworkSlave::getConnectionIdentifier() {
    if (connection_type == "Multicast") return getName();
    else return node->getAddress() + ":" + toString(port);
}



