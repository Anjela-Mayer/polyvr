#include "VRThreadManager.h"
#include "core/utils/VRFunction.h"

#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>

#include <OpenSG/OSGChangeList.h>
#include <OpenSG/OSGThread.h>
#include <OpenSG/OSGThreadManager.h>
#include <OpenSG/OSGGLUT.h>


OSG_BEGIN_NAMESPACE;
using namespace std;

class OSGThread : public ExternalThread {
    public:
        static ExternalThread* create(const char* szName, int uiId) {
            return ExternalThread::create(szName, uiId);
        }
};

void VRThreadManager::runLoop(VRThreadWeakPtr wt) {
    auto t = wt.lock();
    ExternalThreadRefPtr tr = OSGThread::create(t->name.c_str(), 0);
    tr->initialize(t->aspect);//der hauptthread nutzt Aspect 0

    t->osg_t = tr;
    t->status = 1;

    do {
        t = wt.lock();
        if (t) (*t->fkt)(t);
    } while(t->control_flag);

    t->status = 2;
}

VRThreadManager::VRThreadManager() {
    appThread = dynamic_cast<Thread *>(ThreadManager::getAppThread());
}

VRThreadManager::~VRThreadManager() {
    cout << "VRThreadManager::~VRThreadManager()\n";
    for (auto t : threads) stopThread(t.first);
}

VRThread::~VRThread() {
    control_flag = false;
    if (boost_t) {
        boost_t->interrupt();
        delete boost_t;
    }
    if (fkt) delete fkt;
}

void VRThreadManager::stopThread(int id, int tries) {
    if (threads.count(id) == 0) return;
    VRThreadPtr t = threads[id];
    t->control_flag = false;

    int k = 0;
    while (t->status != 2) {
        if (k == tries) { cout << "Warning: thread " << id << " won't stop" << endl; return; }
        k++;
        osgSleep(10);
    }

    threads.erase(id);
}

int VRThreadManager::initThread(VRFunction<VRThreadWeakPtr>* f, string name, bool loop, int aspect) { //start thread
    static int id = 1;

    VRThreadPtr t = VRThreadPtr( new VRThread() );
    t->aspect = aspect;
    t->control_flag = loop;
    t->appThread = appThread;
    t->name = name;
    t->ID = id;
    t->fkt = f;
    t->t_last = glutGet(GLUT_ELAPSED_TIME);
    t->boost_t = new boost::thread(boost::bind(&VRThreadManager::runLoop, this, t));
    threads[id] = t;

    id++;
    return t->ID;
}

void VRThreadManager::killThread(int id) {
    if (threads.count(id) == 0) return;
    cout << "\nKILL THREAD " << id << endl;
    threads.erase(id);
}

int VRThreadManager::getThreadNum() {
    return threads.size();
}

void VRThreadManager::printThreadsStats() {
    cout << "\nActive threads : " << endl;
    for (auto t : threads) cout << " Thread id : " << t.first << " , name : " << t.second->name << endl;
}

void VRThreadManager::ThreadManagerUpdate() {}

OSG_END_NAMESPACE;
