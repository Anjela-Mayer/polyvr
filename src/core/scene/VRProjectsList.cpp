#include "VRProjectsList.h"
#include "core/utils/VRStorage_template.h"

using namespace OSG;

VRProjectEntry::VRProjectEntry(string p, string t) {
    set(p,t);

    store("path", &path);
    store("timestamp", &timestamp);
}

VRProjectEntry::~VRProjectEntry() {}
VRProjectEntryPtr VRProjectEntry::create(string p, string t) { return VRProjectEntryPtr( new VRProjectEntry(p,t) ); }
VRProjectEntryPtr VRProjectEntry::create(string p) { return VRProjectEntryPtr( new VRProjectEntry(p,"") ); }

void VRProjectEntry::set(string p, string t) {
    path = p;
    timestamp = t;
}

string VRProjectEntry::getName() { return path; } // needed for storage map key
string VRProjectEntry::getPath() { return path; }

long VRProjectEntry::getTimestamp() {
    if (timestamp == "") return 0;
    long t;
    toValue(timestamp, t);
    return t;
}

void VRProjectEntry::setTimestamp(string t) { timestamp = t; }



VRProjectsList::VRProjectsList() {
    storeMap("entries", &entries, true);
}

VRProjectsList::~VRProjectsList() {}
VRProjectsListPtr VRProjectsList::create() { return VRProjectsListPtr( new VRProjectsList() ); }

int VRProjectsList::size() { return entries.size(); }

void VRProjectsList::addEntry(VRProjectEntryPtr e) { entries[e->getPath()] = e; }
bool VRProjectsList::hasEntry(string path) { return entries.count(path) != 0; }
void VRProjectsList::remEntry(string path) { if (hasEntry(path)) entries.erase(path); }
void VRProjectsList::clear() { entries.clear(); }
VRProjectEntryPtr VRProjectsList::getEntry(string path) { return hasEntry(path) ? entries[path] : 0; }
map<string, VRProjectEntryPtr> VRProjectsList::getEntries() { return entries; }

vector<string> VRProjectsList::getPaths() {
    vector<string> res;
    for (auto e : entries) res.push_back(e.first);
    return res;
}

vector<VRProjectEntryPtr> VRProjectsList::getEntriesByTimestamp() {
    map<long, map<string, VRProjectEntryPtr> > tmp;
    for (auto e : entries) {
        long dt = time(0) - e.second->getTimestamp();
        if (tmp.count(dt) == 0) tmp[dt] = map<string, VRProjectEntryPtr>();
        tmp[dt][e.first] = e.second;
    }

    vector<VRProjectEntryPtr> res;
    for (auto ev : tmp) {
        for (auto e : ev.second) {
            res.push_back(e.second);
        }
    }

    return res;
}




