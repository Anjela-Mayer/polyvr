#include "VRStorage.h"
#include "toString.h"
#include "VRFunction.h"
#include <libxml++/nodes/element.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

OSG_BEGIN_NAMESPACE;
using namespace std;

template<typename T>
void VRStorage::load_cb(T* t, string tag, xmlpp::Element* e) {
    if (e->get_attribute(tag) == 0) return;
    toValue( e->get_attribute(tag)->get_value(), *t);
}

template<typename T>
void VRStorage::save_cb(T* t, string tag, xmlpp::Element* e) {
    e->set_attribute(tag, toString(*t));
}

template<typename T>
void VRStorage::save_on_cb(T* t, string tag, xmlpp::Element* e) {
    auto sp = t->lock();
    if (sp == 0) return;
    e->set_attribute(tag, sp->getName());
}

template<typename T>
void VRStorage::save_str_map_cb(map<string, T*>* mt, string tag, bool under, xmlpp::Element* e) {
    if (mt->size() == 0) return;
    if (under) e = e->add_child(tag);
    for (auto t : *mt) {
        auto ei = t.second->saveUnder(e);
        if (ei && t.second->overrideCallbacks) t.second->save(ei); // TODO: some classes like VRScript have a custom save method
    }
}

template<typename T>
void VRStorage::save_str_objmap_cb(map<string, std::shared_ptr<T> >* mt, string tag, bool under, xmlpp::Element* e) {
    if (mt->size() == 0) return;
    if (under) e = e->add_child(tag);
    for (auto t : *mt) {
        auto ei = t.second->saveUnder(e);
        if (ei && t.second->overrideCallbacks) t.second->save(ei); // TODO: some classes like VRScript have a custom save method
    }
}

template<typename T>
void VRStorage::save_int_map_cb(map<int, T*>* mt, string tag, bool under, xmlpp::Element* e) {
    if (mt->size() == 0) return;
    if (under) e = e->add_child(tag);
    for (auto t : *mt) {
        auto ei = t.second->saveUnder(e);
        if (ei) ei->set_attribute("ID", toString(t.first));
    }
}

template<typename T>
void VRStorage::save_int_map2_cb(map<int, T>* mt, string tag, bool under, xmlpp::Element* e) {
    if (mt->size() == 0) return;
    if (under) e = e->add_child(tag);
    for (auto t : *mt) {
        auto ei = e->add_child("e");
        if (ei) {
            ei->set_attribute("ID", toString(t.first));
            ei->set_attribute("data", toString(t.second));
        }
    }
}

template<typename T>
void VRStorage::save_int_objmap_cb(map<int, std::shared_ptr<T> >* mt, string tag, bool under, xmlpp::Element* e) {
    if (mt->size() == 0) return;
    if (under) e = e->add_child(tag);
    for (auto t : *mt) {
        auto ei = t.second->saveUnder(e);
        if (ei) ei->set_attribute("ID", toString(t.first));
    }
}

template<typename T>
void VRStorage::save_int_objumap_cb(unordered_map<int, std::shared_ptr<T> >* mt, string tag, bool under, xmlpp::Element* e) {
    if (mt->size() == 0) return;
    if (under) e = e->add_child(tag);
    for (auto t : *mt) {
        auto ei = t.second->saveUnder(e);
        if (ei) ei->set_attribute("ID", toString(t.first));
    }
}

int getID(xmlpp::Element* el);

template<typename T>
void VRStorage::load_int_objmap_cb(map<int, std::shared_ptr<T> >* mt, string tag, bool under, xmlpp::Element* e) {
    if (under) e = getChild(e, tag);
    if (!e) return;
    for (auto el : getChildren(e)) {
        int ID = getID(el);
        if (ID < 0) { cout << "VRStorage::load_int_objmap_cb Error: object " << el->get_name() << " in map '" << tag << "' has no attribute ID!\n"; return; }
        if (mt->count(ID) == 0) {
            auto t = T::create();
            t->load(el);
            (*mt)[ID] = t;
        }
    }
}

template<typename T>
void VRStorage::load_int_objumap_cb(unordered_map<int, std::shared_ptr<T> >* mt, string tag, bool under, xmlpp::Element* e) {
    if (under) e = getChild(e, tag);
    if (!e) return;
    for (auto el : getChildren(e)) {
        int ID = getID(el);
        if (ID < 0) { cout << "VRStorage::load_int_objmap_cb Error: object " << el->get_name() << " in map '" << tag << "' has no attribute ID!\n"; return; }
        if (mt->count(ID) == 0) {
            auto t = T::create();
            t->load(el);
            (*mt)[ID] = t;
        }
    }
}

template<typename T>
void VRStorage::load_str_objmap_cb(map<string, std::shared_ptr<T> >* mt, string tag, bool under, xmlpp::Element* e) {
    if (under) e = getChild(e, tag);
    if (!e) return;
    for (auto el : getChildren(e)) {
        string name = el->get_name();
        if (el->get_attribute("base_name")) name = el->get_attribute("base_name")->get_value();
        auto t = T::create(name);
        t->load(el);

        name = t->getName();
        if (!mt->count(name)) (*mt)[name] = t;
    }
}

template<typename T>
void VRStorage::load_str_objumap_cb(unordered_map<string, std::shared_ptr<T> >* mt, string tag, bool under, xmlpp::Element* e) {
    if (under) e = getChild(e, tag);
    if (!e) return;
    for (auto el : getChildren(e)) {
        string name = el->get_name();
        if (el->get_attribute("base_name")) name = el->get_attribute("base_name")->get_value();
        auto t = T::create(name);
        t->load(el);

        name = t->getName();
        if (!mt->count(name)) (*mt)[name] = t;
    }
}

template<typename T>
void VRStorage::load_str_map_cb(map<string, T*>* mt, string tag, bool under, xmlpp::Element* e) {
    if (under) e = getChild(e, tag);
    if (!e) return;
    for (auto el : getChildren(e)) {
        string name = el->get_name();
        if (el->get_attribute("base_name")) name = el->get_attribute("base_name")->get_value();
        T* o = new T(name);
        o->load(el);

        name = o->getName();
        if (mt->count(name)) delete o;
        else (*mt)[name] = o;
    }
}

template<typename T>
void VRStorage::load_int_map_cb(map<int, T*>* mt, string tag, bool under, xmlpp::Element* e) {
    if (under) e = getChild(e, tag);
    if (!e) return;
    for (auto el : getChildren(e)) {
        int ID = getID(el);
        if (mt->count(ID) == 0) {
            T* o = new T();
            o->load(el);
            (*mt)[ID] = o;
        }
    }
}

template<typename T>
void VRStorage::load_int_map2_cb(map<int, T>* mt, string tag, bool under, xmlpp::Element* e) {
    if (under) e = getChild(e, tag);
    if (!e) return;
    for (auto el : getChildren(e)) {
        int ID = getID(el);
        if (mt->count(ID) == 0) {
            T o;
            toValue( el->get_attribute("data")->get_value(), o);
            (*mt)[ID] = o;
        }
    }
}

template<typename T>
void VRStorage::save_obj_vec_cb(vector<std::shared_ptr<T> >* v, string tag, bool under, xmlpp::Element* e) {
    if (under) e = e->add_child(tag);
    for (auto t : *v) t->saveUnder(e);
}

template<typename T>
void VRStorage::load_obj_vec_cb(vector<std::shared_ptr<T> >* v, string tag, bool under, xmlpp::Element* e) {
    if (e == 0) return;
    if (under) e = getChild(e, tag);
    if (e == 0) return;
    auto children = getChildren(e);
    bool doReload = (v->size() == children.size());
    for (uint i=0; i<children.size(); i++) {
        auto el = children[i];
        if (doReload) (*v)[i]->load(el);
        else {
            VRStoragePtr s = VRStorage::createFromStore(el, false);
            if (!s) s = T::create();
            auto c = static_pointer_cast<T>(s);
            if (!c) continue;
            if (el->get_name() != s->type) continue;
            c->load(el);
            v->push_back( c );
        }
    }
}

template<typename T>
void VRStorage::save_vec_cb(vector<T>* v, string tag, xmlpp::Element* e) {
    e = e->add_child(tag);
    for (auto t : *v) {
        auto e2 = e->add_child("e");
        e2->set_attribute("val", toString(t));
    }
}

template<typename T>
void VRStorage::save_vec_on_cb(vector<T>* v, string tag, xmlpp::Element* e) {
    e = e->add_child(tag);
    for (auto t : *v) {
        auto sp = t.lock();
        if (sp == 0) continue;
        auto e2 = e->add_child("e");
        e2->set_attribute("val", sp->getName());
    }
}

template<typename T>
void VRStorage::load_vec_cb(vector<T>* v, string tag, xmlpp::Element* e) {
    if (e == 0) return;
    e = getChild(e, tag);
    if (e == 0) return;
    v->clear();
    for (auto el : getChildren(e)) {
        if (el->get_attribute("val") == 0) return;
        T t;
        toValue( el->get_attribute("val")->get_value(), t);
        v->push_back( t );
    }
}

template<typename T>
void VRStorage::save_vec_vec_cb(vector<vector<T>>* v, string tag, xmlpp::Element* e) {
    e = e->add_child(tag);
    for (auto t : *v) save_vec_cb(&t, tag, e);
}

template<typename T>
void VRStorage::load_vec_vec_cb(vector<vector<T>>* v, string tag, xmlpp::Element* e) {
    if (e == 0) return;
    e = getChild(e, tag);
    if (e == 0) return;
    v->clear();
    for (auto el : getChildren(e)) {
        v->push_back(vector<T>());
        vector<T>* vv = &v->at(v->size()-1);
        for (auto el2 : getChildren(el)) {
            if (el2->get_attribute("val") == 0) continue;
            T t;
            toValue( el2->get_attribute("val")->get_value(), t);
            vv->push_back( t );
        }
    }
}

template<typename T>
void VRStorage::save_obj_cb(std::shared_ptr<T>* v, string tag, xmlpp::Element* e) {
    if (*v) (*v)->saveUnder(e, 0, tag);
}

template<typename T>
void VRStorage::load_obj_cb(std::shared_ptr<T>* v, string tag, xmlpp::Element* e) {
    e = getChild(e, tag);
    if (!e) return;
    if (!*v) *v = T::create();
    (*v)->load(e);
}

template<typename T>
void VRStorage::storeObj(string tag, std::shared_ptr<T>& o) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_obj_cb<T>, this, &o, tag, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_obj_cb<T>, this, &o, tag, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeObjVec(string tag, vector<std::shared_ptr<T> >& v, bool under) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_obj_vec_cb<T>, this, &v, tag, under, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_obj_vec_cb<T>, this, &v, tag, under, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeVec(string tag, vector<T>& v) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_vec_cb<T>, this, &v, tag, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_vec_cb<T>, this, &v, tag, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeVecVec(string tag, vector<vector<T>>& v){
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_vec_vec_cb<T>, this, &v, tag, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_vec_vec_cb<T>, this, &v, tag, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::store(string tag, T* t) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_cb<T>, this, t, tag, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_cb<T>, this, t, tag, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeObjName(string tag, T* o, string* t) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_cb<string>, this, t, tag, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_on_cb<T>, this, o, tag, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeObjNames(string tag, vector<T>* o, vector<string>* v) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_vec_cb<string>, this, v, tag, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_vec_on_cb<T>, this, o, tag, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeMap(string tag, unordered_map<string, std::shared_ptr<T> >* mt, bool under) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_str_objumap_cb<T>, this, mt, tag, under, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_str_objumap_cb<T>, this, mt, tag, under, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeMap(string tag, unordered_map<int, std::shared_ptr<T> >* mt, bool under) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_int_objumap_cb<T>, this, mt, tag, under, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_int_objumap_cb<T>, this, mt, tag, under, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeMap(string tag, map<string, std::shared_ptr<T> >* mt, bool under) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_str_objmap_cb<T>, this, mt, tag, under, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_str_objmap_cb<T>, this, mt, tag, under, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeMap(string tag, map<int, std::shared_ptr<T> >* mt, bool under) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_int_objmap_cb<T>, this, mt, tag, under, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_int_objmap_cb<T>, this, mt, tag, under, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeMap(string tag, map<string, T*>* mt, bool under) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_str_map_cb<T>, this, mt, tag, under, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_str_map_cb<T>, this, mt, tag, under, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeMap(string tag, map<int, T*>* mt, bool under) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_int_map_cb<T>, this, mt, tag, under, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_int_map_cb<T>, this, mt, tag, under, _1 ) );
    storage[tag] = b;
}

template<typename T>
void VRStorage::storeMap(string tag, map<int, T>& mt) {
    VRStorageBin b;
    b.f1 = VRStoreCb::create("load", boost::bind( &VRStorage::load_int_map2_cb<T>, this, &mt, tag, true, _1 ) );
    b.f2 = VRStoreCb::create("save", boost::bind( &VRStorage::save_int_map2_cb<T>, this, &mt, tag, true, _1 ) );
    storage[tag] = b;
}

template<class T>
void VRStorage::typeFactoryCb(VRStoragePtr& s) { s = T::create(); }

template<class T>
void VRStorage::regStorageType(string t) {
    factory[t] = VRStorageFactoryCb::create("factorycb", boost::bind( &VRStorage::typeFactoryCb<T>, _1 ) );
}

OSG_END_NAMESPACE;
