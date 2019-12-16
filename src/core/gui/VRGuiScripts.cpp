#include "VRGuiScripts.h"
#include "VRGuiUtils.h"
#include "VRGuiFile.h"
#include "VRGuiBits.h"
#include "VRGuiManager.h"
#include "VRGuiConsole.h"
#include "core/setup/VRSetup.h"
#include "core/scene/VRScene.h"
#include "core/scripting/VRScript.h"
#include "core/utils/toString.h"
#include "core/utils/VRTimer.h"
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/textview.h>
#include <gtkmm/combobox.h>
#include <gtkmm/dialog.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treestore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/table.h>
#include <gtkmm/paned.h>
#include <gtkmm/builder.h>
#include <gtkmm/filechooser.h>
#include <gtk/gtktextview.h>
#include <libxml++/nodes/element.h>
#include <libxml++/libxml++.h>

OSG_BEGIN_NAMESPACE;
using namespace std;
using namespace Gtk;

class VRGuiScripts_ModelColumns : public Gtk::TreeModelColumnRecord {
    public:
        VRGuiScripts_ModelColumns() { add(script); add(fg); add(bg); add(time); add(tfg); add(tbg); add(icon); add(Nfound); add(type); }
        Gtk::TreeModelColumn<Glib::ustring> script;
        Gtk::TreeModelColumn<Glib::ustring> fg;
        Gtk::TreeModelColumn<Glib::ustring> bg;
        Gtk::TreeModelColumn<Glib::ustring> time;
        Gtk::TreeModelColumn<Glib::ustring> tfg;
        Gtk::TreeModelColumn<Glib::ustring> tbg;
        Gtk::TreeModelColumn<Glib::ustring> icon;
        Gtk::TreeModelColumn<Glib::ustring> Nfound;
        Gtk::TreeModelColumn<gint> type;
};

class VRGuiScripts_ArgsModelColumns : public Gtk::TreeModelColumnRecord {
    public:
        VRGuiScripts_ArgsModelColumns() { add(name); add(value); add(obj); add(type); }
        Gtk::TreeModelColumn<Glib::ustring> name;
        Gtk::TreeModelColumn<Glib::ustring> value;
        Gtk::TreeModelColumn<gpointer> obj;
        Gtk::TreeModelColumn<Glib::ustring> type;
};

class VRGuiScripts_TrigsModelColumns : public Gtk::TreeModelColumnRecord {
    public:
        VRGuiScripts_TrigsModelColumns() { add(trigs); add(devs); add(key); add(state); add(params); add(obj); add(name); }
        Gtk::TreeModelColumn<Glib::ustring> trigs;
        Gtk::TreeModelColumn<Glib::ustring> devs;
        Gtk::TreeModelColumn<Glib::ustring> key;
        Gtk::TreeModelColumn<Glib::ustring> state;
        Gtk::TreeModelColumn<Glib::ustring> params;
        Gtk::TreeModelColumn<gpointer> obj;
        Gtk::TreeModelColumn<Glib::ustring> name;
};

class VRGuiScripts_HelpColumns : public Gtk::TreeModelColumnRecord {
    public:
        VRGuiScripts_HelpColumns() { add(obj); add(type); add(cla); add(mod); }
        Gtk::TreeModelColumn<Glib::ustring> obj;
        Gtk::TreeModelColumn<Glib::ustring> type;
        Gtk::TreeModelColumn<Glib::ustring> cla;
        Gtk::TreeModelColumn<Glib::ustring> mod;
};

// --------------------------
// ---------Callbacks--------
// --------------------------

VRScriptPtr VRGuiScripts::getSelectedScript() {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview5"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return 0;

    // get selected script
    VRGuiScripts_ModelColumns cols;
    Gtk::TreeModel::Row row = *iter;
    string name = row.get_value(cols.script);
    auto scene = VRScene::getCurrent();
    if (scene == 0) return 0;
    VRScriptPtr script = scene->getScript(name);

    return script;
}

void VRGuiScripts::setGroupListRow(Gtk::TreeIter itr, group& g) {
    Glib::RefPtr<Gtk::TreeStore> store = Glib::RefPtr<Gtk::TreeStore>::cast_static(VRGuiBuilder()->get_object("script_tree"));
    Gtk::TreeStore::Row row = *itr;
    gtk_tree_store_set (store->gobj(), row.gobj(),
                        0, g.name.c_str(),
                        1, "#FFFFFF",
                        2, "#444444",
                        3, "",
                        4, "#666666",
                        5, "#FFFFFF",
                        6, "",
                        7, "",
                        8, g.ID,
                        -1);
}

void VRGuiScripts::setScriptListRow(Gtk::TreeIter itr, VRScriptPtr script, bool onlyTime) {
    if (script == 0) return;
    string color = "#000000";
    string background = "#FFFFFF";
    string tfg = "#000000";
    string tbg = "#FFFFFF";

    if (script->getPersistency() == 0) { // imported script
        color = "#0000FF";
    }

    int trig_lvl = 0;
    for (auto trig : script->getTriggers()) {
        if (trig->trigger == "on_scene_load") trig_lvl |= 1;
        if (trig->trigger == "on_scene_close") trig_lvl |= 2;
        if (trig->trigger == "on_scene_import") trig_lvl |= 1;
        if (trig->trigger == "on_timeout") trig_lvl |= 4;
        if (trig->trigger == "on_device") trig_lvl |= 8;
        if (trig->trigger == "on_socket") trig_lvl |= 16;
        if (trig->trigger == "on_device_drag") trig_lvl |= 32;
        if (trig->trigger == "on_device_drop") trig_lvl |= 64;
    }
    if (script->getType() == "HTML") trig_lvl |= 128;
    if (script->getType() == "GLSL") trig_lvl |= 256;

    if (trig_lvl >= 1) tbg = "#AAFF88";
    if (trig_lvl >= 4) tbg = "#FF8866";
    if (trig_lvl >= 8) tbg = "#FFBB33";
    if (trig_lvl >= 16) tbg = "#3388FF";
    if (trig_lvl >= 32) tbg = "#FFCCAA";
    if (trig_lvl >= 64) tbg = "#FFCC88";
    if (trig_lvl >= 128) tbg = "#AACCFF";
    if (trig_lvl >= 256) tbg = "#CCAAFF";

    string time = " ";
    float exec_time = script->getExecutionTime();
    if (exec_time >= 60*1000) time = toString( exec_time*0.001/60 ) + " min";
    else if (exec_time >= 1000) time = toString( exec_time*0.001 ) + " s";
    else if (exec_time >= 0) time = toString( exec_time ) + " ms";

    Gtk::Window* win1;
    VRGuiBuilder()->get_widget("window1", win1);
    Gtk::Widget* wdg = 0;
    if (win1) wdg = win1->get_focus();
    string name;
    if (wdg) name = wdg->get_name();
    bool user_focus = false;
    if(!user_focus) user_focus = ("gtkmm__GtkTreeView" == name);
    if(!user_focus) user_focus = ("GtkEntry" == name); // TODO: be more specific
    if(onlyTime && (user_focus || !doPerf)) return;

    int Nf = script->getSearch().N;
    string icon, Nfound;
    if (Nf) {
        icon = "gtk-find";
        Nfound = toString(Nf);
    }

    Glib::RefPtr<Gtk::TreeStore> store = Glib::RefPtr<Gtk::TreeStore>::cast_static(VRGuiBuilder()->get_object("script_tree"));
    Gtk::TreeStore::Row row = *itr;
    if (onlyTime) gtk_tree_store_set (store->gobj(), row.gobj(),
                        3, time.c_str(),
                        4, tfg.c_str(),
                        5, tbg.c_str(),
                        -1);
    else gtk_tree_store_set (store->gobj(), row.gobj(),
                        0, script->getName().c_str(),
                        1, color.c_str(),
                        2, background.c_str(),
                        3, time.c_str(),
                        4, tfg.c_str(),
                        5, tbg.c_str(),
                        6, icon.c_str(),
                        7, Nfound.c_str(),
                        8, -1,
                        -1);
}

void VRGuiScripts::on_new_clicked() {
    auto scene = VRScene::getCurrent();
    if (scene == 0) return;
    scene->newScript("Script", "\timport VR");
    updateList();
}

VRGuiScripts::group::group() { static int i = 0; ID = i; i++; }

void VRGuiScripts::on_addSep_clicked() {
    group g;
    groups[g.ID] = g;
    updateList();
}

void VRGuiScripts::on_save_clicked() {
    VRScriptPtr script = getSelectedScript();
    if (script == 0) return;

    string core = editor->getCore(script->getHeadSize());
    auto scene = VRScene::getCurrent();
    if (scene == 0) return;
    scene->updateScript(script->getName(), core);

    setToolButtonSensitivity("toolbutton7", false);

    saveScene();
}

void VRGuiScripts::on_import_clicked() {
    auto scene = VRScene::getCurrent();
    if (scene == 0) return;

    import_liststore1->clear();
    import_liststore2->clear();
    ListStore::Row row;
    for (auto script : scene->getScripts()) {
        row = *import_liststore2->append();
        gtk_list_store_set (import_liststore2->gobj(), row.gobj(), 0, script.first.c_str(), -1);
    }

    VRGuiFile::clearFilter();
    VRGuiFile::addFilter("Project", 2, "*.xml", "*.pvr");
    VRGuiFile::addFilter("All", 1, "*");
    VRGuiFile::setWidget(scriptImportWidget, true, true);
    VRGuiFile::setCallbacks( sigc::mem_fun(*this, &VRGuiScripts::on_diag_import), sigc::slot<void>(), sigc::mem_fun(*this, &VRGuiScripts::on_diag_import_select));
    VRGuiFile::open("Import", Gtk::FILE_CHOOSER_ACTION_OPEN, "Import script");
}

XMLElementPtr getXMLChild(XMLElementPtr e, string name) {
    for (auto n : e->get_children()) {
        XMLElementPtr el = dynamic_cast<XMLElementPtr>(n);
        if (!el) continue;
        if (el->get_name() == name) return el;
    } return 0;
}

vector<XMLElementPtr> getXMLChildren(XMLElementPtr e) {
    vector<XMLElementPtr> res;
    for (auto n : e->get_children()) {
        XMLElementPtr el = dynamic_cast<XMLElementPtr>(n);
        if (!el) continue;
        res.push_back(el);
    }
    return res;
}

void VRGuiScripts::on_diag_import_select() {
    import_liststore1->clear();
    import_scripts.clear();
    string path = VRGuiFile::getPath();
    if (path == "") return;

    xmlpp::DomParser parser;
    parser.set_validate(false);
    try { parser.parse_file(path.c_str()); }
    catch(exception& e) { return; }

    xmlpp::Node* n = parser.get_document()->get_root_node();
    XMLElementPtr scene = dynamic_cast<XMLElementPtr>(n);
    vector<XMLElementPtr> scripts = getXMLChildren( getXMLChild(scene, "Scripts") );

    ListStore::Row row;
    for (auto script : scripts) {
        string name = script->get_name();
        if (script->get_attribute("base_name")) {
            string suffix = script->get_attribute("name_suffix")->get_value();
            if (suffix == "0") suffix = "";
            name = script->get_attribute("base_name")->get_value() + suffix;
        }

        VRScriptPtr s = VRScript::create(name);
        s->enable(false);
        s->load(script);
        import_scripts[name] = s;

        row = *import_liststore1->append();
        gtk_list_store_set (import_liststore1->gobj(), row.gobj(), 0, name.c_str(), -1);
    }
}

void VRGuiScripts::on_diag_import() {
    auto itr = import_treeview1->get_selection()->get_selected();
    if (!itr) return;

    // get selected script
    VRGuiScripts_ModelColumns cols;
    Gtk::TreeModel::Row row = *itr;
    string name = row.get_value(cols.script);
    if (import_scripts.count(name) == 0) return;

    VRScriptPtr s = import_scripts[name];
    import_scripts.erase(name);

    auto scene = VRScene::getCurrent();
    if (scene == 0) return;
    scene->addScript(s);
    s->enable(true);
    updateList();
}

void VRGuiScripts::on_diag_import_select_1() {} // TODO
void VRGuiScripts::on_diag_import_select_2() {}

void VRGuiScripts::on_exec_clicked() {
    VRScriptPtr script = getSelectedScript();
    if (script == 0) return;

    //VRTimer t; t.start();
    on_save_clicked();
    //cout << " VRGuiScripts::on_exec_clicked t1 " << t.stop() << endl; t.reset();

    auto scene = VRScene::getCurrent();
    if (scene == 0) return;

    scene->triggerScript(script->getName());
    //cout << " VRGuiScripts::on_exec_clicked t2 " << t.stop() << endl; t.reset();
    //VRGuiSignals::get()->getSignal("scene_modified")->triggerPtr<VRDevice>(); // realy needed? can take a lot of time!
    //cout << " VRGuiScripts::on_exec_clicked t3 " << t.stop() << endl; t.reset();
}

void VRGuiScripts::on_perf_toggled() {
    doPerf = getToggleButtonState("toggletoolbutton1");
}

void VRGuiScripts::on_pause_toggled() {
    bool b = getToggleButtonState("toggletoolbutton2");
    VRScene::getCurrent()->pauseScripts(b);
}

void VRGuiScripts::on_del_clicked() {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview5"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // get selected script
    VRGuiScripts_ModelColumns cols;
    Gtk::TreeModel::Row row = *iter;
    string name = row.get_value(cols.script);
    auto scene = VRScene::getCurrent();
    if (scene == 0) return;
    VRScriptPtr script = scene->getScript(name);
    if (script == 0) return;

    string msg1 = "Delete script " + name;
    if (!askUser(msg1, "Are you sure you want to delete this script?")) return;

    scene->remScript(script->getName());
    updateList();

    setToolButtonSensitivity("toolbutton9", false);
    setToolButtonSensitivity("toolbutton8", false);
}

void VRGuiScripts::on_select_script() { // selected a script
    VRScriptPtr script = VRGuiScripts::getSelectedScript();
    if (script == 0) {
        setToolButtonSensitivity("toolbutton8", false);
        setToolButtonSensitivity("toolbutton7", false);
        setToolButtonSensitivity("toolbutton9", false);
        setTableSensitivity("table15", false);
        return;
    }

    trigger_cbs = false;

    // update options
    setCombobox("combobox1", getListStorePos("liststore6", script->getType()));
    auto setup = VRSetup::getCurrent();
    if (setup) fillStringListstore("liststore7", setup->getDevices("server"));
    vector<string> grps;
    grps.push_back("no group");
    for (auto g : groups) grps.push_back(g.second.name);
    fillStringListstore("liststore10", grps);
    setCombobox("combobox24", getListStorePos("liststore7", script->getServer()));
    setCombobox("combobox10", getListStorePos("liststore10", script->getGroup()));

    // update editor content
    editor->setCore(script->getScript());
    //adjustment->set_value(pages[script.get()].line);

    // update arguments liststore
    Glib::RefPtr<Gtk::ListStore> args = Glib::RefPtr<Gtk::ListStore>::cast_static(VRGuiBuilder()->get_object("liststore2"));
    args->clear();

    //if (PyErr_Occurred() != NULL) PyErr_Print();
    for (auto a : script->getArguments()) {
        Gtk::ListStore::Row row = *args->append();
        gtk_list_store_set(args->gobj(), row.gobj(), 0, a->getName().c_str(), -1);
        gtk_list_store_set(args->gobj(), row.gobj(), 1, a->val.c_str(), -1);
        gtk_list_store_set(args->gobj(), row.gobj(), 2, script.get(), -1);
        gtk_list_store_set(args->gobj(), row.gobj(), 3, a->type.c_str(), -1);
    }

    // update trigger liststore
    Glib::RefPtr<Gtk::ListStore> trigs = Glib::RefPtr<Gtk::ListStore>::cast_static(VRGuiBuilder()->get_object("triggers"));
    trigs->clear();
    for (auto t : script->getTriggers()) {
        string key = toString(t->key);
        if (t->dev == "keyboard" && t->key > 32 && t->key < 127) {
            char kc = t->key;
            key = kc;
        }
        Gtk::ListStore::Row row = *trigs->append();
        gtk_list_store_set(trigs->gobj(), row.gobj(), 0, t->trigger.c_str(), -1);
        gtk_list_store_set(trigs->gobj(), row.gobj(), 1, t->dev.c_str(), -1);
        gtk_list_store_set(trigs->gobj(), row.gobj(), 2, key.c_str(), -1);
        gtk_list_store_set(trigs->gobj(), row.gobj(), 3, t->state.c_str(), -1);
        gtk_list_store_set(trigs->gobj(), row.gobj(), 4, t->param.c_str(), -1);
        gtk_list_store_set(trigs->gobj(), row.gobj(), 5, script.get(), -1);
        gtk_list_store_set(trigs->gobj(), row.gobj(), 6, t->getName().c_str(), -1);
    }

    setToolButtonSensitivity("toolbutton8", true);
    setToolButtonSensitivity("toolbutton7", false);
    setToolButtonSensitivity("toolbutton9", true);
    setTableSensitivity("table15", true);

    // language
    editor->setLanguage(script->getType());

    // script trigger
    //string trigger = script->getTrigger();
    //setTextEntry("entry48", script->getTriggerParams());

    //setCombobox("combobox1", getListStorePos("ScriptTrigger", trigger));
    trigger_cbs = true;
}

// keyboard key detection
bool wait_for_key = false;
Gtk::TreeModel::Row trigger_row;
void VRGuiScripts::on_select_trigger() {
    wait_for_key = false;

    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview14"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    //pixbuf pressed?
    Gtk::TreeModel::Path p;
    Gtk::TreeViewColumn* c;
    tree_view->get_cursor(p,c);
    string col_name = c->get_title();
    if (col_name != "") return;

    // get key
    VRGuiScripts_TrigsModelColumns cols;
    trigger_row = *iter;
    string device = trigger_row.get_value(cols.devs);
    if (device != "keyboard") return;

    wait_for_key = true;
}

bool VRGuiScripts::on_any_key_event(GdkEventKey* event) {
    if (!wait_for_key) return false;
    wait_for_key = false;

    int key = event->keyval;
    VRGuiScripts_TrigsModelColumns cols;
    trigger_row[cols.key] = toString(key);

    VRScript* script = (VRScript*)trigger_row.get_value(cols.obj);
    script->changeTrigKey(trigger_row.get_value(cols.name), key);
    on_select_script();
    on_save_clicked();

    return true;
}

bool VRGuiScripts::on_any_event(GdkEvent* event) {
    int t = event->type;
    if (t == GDK_KEY_PRESS) return on_any_key_event((GdkEventKey*)event);
    return false;
}


void VRGuiScripts::on_name_edited(const Glib::ustring& path, const Glib::ustring& new_name) {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview5"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if (!iter) return;

    // set the cell with new name
    VRGuiScripts_ModelColumns cols;
    Gtk::TreeModel::Row row = *iter;
    string name = row.get_value(cols.script);
    row[cols.script] = new_name;

    // update data
    int type = row.get_value(cols.type);
    if (type == -1) {
        auto scene = VRScene::getCurrent();
        if (scene == 0) return;
        scene->changeScriptName(name, new_name);
    } else {
        cout << "VRGuiScripts::on_name_edited grp ID " << type << endl;
        groups[type].name = new_name;
        for (auto& sw : groups[type].scripts) if (auto s = sw.lock()) s->setGroup(new_name);
    }
    updateList();
    on_select_script();
}

void VRGuiScripts_on_script_changed(GtkTextBuffer* tb, gpointer user_data) {
    setToolButtonSensitivity("toolbutton7", true);

    VRGuiScripts* gs = (VRGuiScripts*)user_data;

    VRScriptPtr script = gs->getSelectedScript();
    if (script == 0) return;

    // TODO
    // get in which line the changed occured
    // negate change if in line 0

    string core = gs->getEditor()->getCore(script->getHeadSize());
    auto scene = VRScene::getCurrent();
    if (scene == 0) return;
    scene->updateScript(script->getName(), core, false);
}

shared_ptr<VRGuiEditor> VRGuiScripts::getEditor() { return editor; }

void VRGuiScripts::on_argadd_clicked() {
    VRScriptPtr script = getSelectedScript();
    if (script == 0) return;

    script->addArgument();
    on_select_script();
    on_save_clicked();
}

void VRGuiScripts::on_trigadd_clicked() {
    VRScriptPtr script = getSelectedScript();
    if (script == 0) return;

    script->addTrigger();
    on_select_script();
    on_save_clicked();
}

void VRGuiScripts::on_argrem_clicked() {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview7"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // set the cell with new name
    VRGuiScripts_ArgsModelColumns cols;
    Gtk::TreeModel::Row row = *iter;
    string name = row.get_value(cols.name);
    auto script = (VRScript*)row.get_value(cols.obj);

    script->remArgument(name);
    on_select_script();
    on_save_clicked();
}

void VRGuiScripts::on_trigrem_clicked() {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview14"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // set the cell with new name
    VRGuiScripts_TrigsModelColumns cols;
    Gtk::TreeModel::Row row = *iter;
    string name = row.get_value(cols.name);
    auto script = (VRScript*)row.get_value(cols.obj);

    script->remTrigger(name);
    on_select_script();
    on_save_clicked();
}

void VRGuiScripts::on_argname_edited(const Glib::ustring& path, const Glib::ustring& new_name) {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview7"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // set the cell with new name
    VRGuiScripts_ArgsModelColumns cols;
    Gtk::TreeModel::Row row = *iter;
    string name = row.get_value(cols.name);
    row[cols.name] = new_name;
    auto script = (VRScript*)row.get_value(cols.obj);

    // update argument name
    script->changeArgName(name, new_name);
    on_select_script();
    on_save_clicked();
}

void VRGuiScripts::on_argval_edited(const Glib::ustring& path, const Glib::ustring& new_name) {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview7"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // set the cell with new value
    VRGuiScripts_ArgsModelColumns cols;
    Gtk::TreeModel::Row row = *iter;
    //string value = row.get_value(cols.value);
    string name = row.get_value(cols.name);
    row[cols.value] = new_name;
    auto script = (VRScript*)row.get_value(cols.obj);

    // update argument name
    script->changeArgValue(name, new_name);
    on_select_script();
    on_save_clicked();
}

void VRGuiScripts::on_argtype_edited(const Glib::ustring& new_name, const Gtk::TreeModel::iterator& new_iter) {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview7"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // set the cell with new type
    Glib::RefPtr<Gtk::ListStore> combo_list = Glib::RefPtr<Gtk::ListStore>::cast_static(VRGuiBuilder()->get_object("arg_types"));
    gchar *t;
    gtk_tree_model_get((GtkTreeModel*)combo_list->gobj(), (GtkTreeIter*)new_iter->gobj(), 0, &t, -1);
    string type = string(t);
    Gtk::TreeModel::Row row = *iter;
    VRGuiScripts_ArgsModelColumns cols;
    row[cols.type] = type;

    // do something
    auto script = (VRScript*)row.get_value(cols.obj);
    script->changeArgType(row.get_value(cols.name), type);
    on_select_script();
    on_save_clicked();
}

void VRGuiScripts::on_trigger_edited(const Glib::ustring& new_name, const Gtk::TreeModel::iterator& new_iter) {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview14"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // set the cell with new type
    Glib::RefPtr<Gtk::ListStore> combo_list = Glib::RefPtr<Gtk::ListStore>::cast_static(VRGuiBuilder()->get_object("ScriptTrigger"));
    gchar *t;
    gtk_tree_model_get((GtkTreeModel*)combo_list->gobj(), (GtkTreeIter*)new_iter->gobj(), 0, &t, -1);
    string type = string(t);
    Gtk::TreeModel::Row row = *iter;
    VRGuiScripts_TrigsModelColumns cols;
    row[cols.trigs] = type;

    // do something
    auto script = (VRScript*)row.get_value(cols.obj);
    script->changeTrigger(row.get_value(cols.name), type);
    on_select_script();
    on_save_clicked();
}

void VRGuiScripts::on_trigdev_edited(const Glib::ustring& new_name, const Gtk::TreeModel::iterator& new_iter) {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview14"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // set the cell with new type
    Glib::RefPtr<Gtk::ListStore> combo_list = Glib::RefPtr<Gtk::ListStore>::cast_static(VRGuiBuilder()->get_object("ScriptTriggerDevices"));
    gchar *t;
    gtk_tree_model_get((GtkTreeModel*)combo_list->gobj(), (GtkTreeIter*)new_iter->gobj(), 0, &t, -1);
    string type = string(t);
    Gtk::TreeModel::Row row = *iter;
    VRGuiScripts_TrigsModelColumns cols;
    row[cols.devs] = type;

    // do something
    auto script = (VRScript*)row.get_value(cols.obj);
    script->changeTrigDev(row.get_value(cols.name), type);
    on_select_script();
    on_save_clicked();
}


void VRGuiScripts::on_trigparam_edited(const Glib::ustring& path, const Glib::ustring& new_name) {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview14"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // set the cell with new value
    VRGuiScripts_TrigsModelColumns cols;
    Gtk::TreeModel::Row row = *iter;
    row[cols.params] = new_name;

    // do something
    auto script = (VRScript*)row.get_value(cols.obj);
    script->changeTrigParams(row.get_value(cols.name), new_name);
    on_select_script();
    on_save_clicked();
}

void VRGuiScripts::on_trigkey_edited(const Glib::ustring& path, const Glib::ustring& new_name) {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview14"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // set the cell with new value
    VRGuiScripts_TrigsModelColumns cols;
    Gtk::TreeModel::Row row = *iter;
    row[cols.key] = new_name;

    // do something
    auto script = (VRScript*)row.get_value(cols.obj);
    script->changeTrigKey(row.get_value(cols.name), toInt(new_name));
    on_select_script();
    on_save_clicked();
}

void VRGuiScripts::on_trigstate_edited(const Glib::ustring& new_name, const Gtk::TreeModel::iterator& new_iter) {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview14"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // set the cell with new type
    Glib::RefPtr<Gtk::ListStore> combo_list = Glib::RefPtr<Gtk::ListStore>::cast_static(VRGuiBuilder()->get_object("ScriptTriggerStates"));
    gchar *t;
    gtk_tree_model_get((GtkTreeModel*)combo_list->gobj(), (GtkTreeIter*)new_iter->gobj(), 0, &t, -1);
    string type = string(t);
    Gtk::TreeModel::Row row = *iter;
    VRGuiScripts_TrigsModelColumns cols;
    row[cols.state] = type;

    // do something
    auto script = (VRScript*)row.get_value(cols.obj);
    script->changeTrigState(row.get_value(cols.name), type);
    on_select_script();
    on_save_clicked();
}

// help dialog

void VRGuiScripts::on_help_close_clicked() {
    Gtk::Dialog* diag;
    VRGuiBuilder()->get_widget("pybindings-docs", diag);
    diag->hide();
}

void VRGuiScripts::on_help_clicked() {
    VRGuiScripts::updateDocumentation();

    Glib::RefPtr<Gtk::TextBuffer> tb  = Glib::RefPtr<Gtk::TextBuffer>::cast_static(VRGuiBuilder()->get_object("pydoc"));
    tb->set_text("");

    Gtk::Dialog* diag;
    VRGuiBuilder()->get_widget("pybindings-docs", diag);
    diag->show();

    //diag->deiconify();
    //diag->raise();
    //diag->show();
    //diag->activate_focus();

    focusEntry("entry25");
}

void VRGuiScripts::updateDocumentation() {
    Glib::RefPtr<Gtk::TreeStore> store = Glib::RefPtr<Gtk::TreeStore>::cast_static(VRGuiBuilder()->get_object("bindings"));
    Glib::RefPtr<Gtk::TreeView> view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview3"));
    store->clear();

    auto scene = VRScene::getCurrent();
    if (scene == 0) return;

    // apply filter
    map<string, map<string, map<string, bool> > > data;
    map<string, map<string, map<string, bool> > > filtered;

    auto sameChar = [](char c1, char c2) { return std::toupper(c1) == std::toupper(c2); };

    auto contain = [&](const string& s, const string& i) {
        return bool( search( s.begin(), s.end(), i.begin(), i.end(), sameChar ) != s.end() );
    };

    for (auto mod : scene->getPyVRModules()) {
        data[mod] = map<string, map<string, bool> >();
        for (auto typ : scene->getPyVRTypes(mod)) {
            data[mod][typ] = map<string, bool>();
            for (auto fkt : scene->getPyVRMethods(mod, typ) ) data[mod][typ][fkt] = 1;
        }
    }

    filtered = data;
    if (docs_filter != "") {
        for (auto mod : data) {
            bool showMod = contain(mod.first, docs_filter);
            if (!showMod) {
                for (auto typ : mod.second) {
                    bool showTyp = contain(typ.first, docs_filter);
                    if (!showTyp) {
                        for (auto fkt : typ.second) {
                            bool showFkt = contain(fkt.first, docs_filter);
                            if (showFkt) showTyp = true;
                            else filtered[mod.first][typ.first].erase(fkt.first);
                        }
                    }
                    if (showTyp) showMod = true;
                    else filtered[mod.first].erase(typ.first);
                }
            }
            if (!showMod) filtered.erase(mod.first);
        }
    }

    auto setRow = [&](Gtk::TreeModel::iterator itr, string label, string type, string cla, string mod, string col = "#FFFFFF") {
        Gtk::TreeStore::Row row = *itr;
        gtk_tree_store_set (store->gobj(), row.gobj(),  0, label.c_str(),
                                                        1, type.c_str(),
                                                        2, cla.c_str(),
                                                        3, mod.c_str(),
                                                        4, col.c_str(), -1);
    };

    Gtk::TreeModel::iterator itr0, itr1, itr2;
    for (auto mod : filtered) {
        itr0 = store->append();
        string modname = (mod.first == "VR") ? "VR" : "VR."+mod.first;
        setRow(itr0, modname, "module", "", "", "#BBDDFF");
        for (auto typ : mod.second) {
            itr1 = store->append(itr0->children());
            setRow(itr1, typ.first, "class", typ.first, mod.first);
            for (auto fkt : typ.second) {
                itr2 = store->append(itr1->children());
                setRow(itr2, fkt.first, "method", typ.first, mod.first);
            }
        }
    }

    if (docs_filter != "") view->expand_all();
}

void VRGuiScripts::on_select_help() {
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview3"));
    Gtk::TreeModel::iterator iter = tree_view->get_selection()->get_selected();
    if(!iter) return;

    // get selected object
    VRGuiScripts_HelpColumns cols;
    Gtk::TreeModel::Row row = *iter;
    string obj = row.get_value(cols.obj);
    string type = row.get_value(cols.type);
    string cla = row.get_value(cols.cla);
    string mod = row.get_value(cols.mod);

    auto scene = VRScene::getCurrent();
    if (scene == 0) return;
    Glib::RefPtr<Gtk::TextBuffer> tb  = Glib::RefPtr<Gtk::TextBuffer>::cast_static(VRGuiBuilder()->get_object("pydoc"));

    tb->set_text(string());
    if (type == "class") {
        string doc = scene->getPyVRDescription(mod, obj) + "\n\n";
        for (auto method : scene->getPyVRMethods(mod, obj)) {
            string d = scene->getPyVRMethodDoc(mod, cla, method);
            doc += method + "\n\t" + d + "\n\n";
        }
        tb->set_text(doc);
    }

    if (type == "method") {
        string doc = "\n" + scene->getPyVRMethodDoc(mod, cla, obj);
        tb->set_text(doc);
    }
}

void VRGuiScripts::on_doc_filter_edited() {
    docs_filter = getTextEntry("entry25");
    updateDocumentation();
}

// script search dialog

void VRGuiScripts::on_find_clicked() {
    setCheckButton("checkbutton38", true);
    setEntrySensitivity("entry11", false);
    string txt = editor->getSelection();
    setTextEntry("entry10", txt);
    focusEntry("entry10");
    showDialog("find_dialog");
}

void VRGuiScripts::on_find_diag_cancel_clicked() {
    hideDialog("find_dialog");
}

VRGuiScripts::searchResult::searchResult(string s, int l, int c) : scriptName(s), line(l), column(c) {}

void VRGuiScripts::focusScript(string name, int line, int column) {
    setNotebookPage("notebook1", 2);
    setNotebookPage("notebook3", 3);

    Glib::RefPtr<Gtk::TreeStore> store = Glib::RefPtr<Gtk::TreeStore>::cast_static(VRGuiBuilder()->get_object("script_tree"));
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview5"));

    auto selectScript = [&](Gtk::TreeIter& itr) {
        Gtk::TreeModel::Row row = *itr;
        VRGuiScripts_ModelColumns cols;
        if (name == row.get_value(cols.script)) {
            Gtk::TreeModel::Path path(itr);
            tree_view->expand_to_path(path);
            tree_view->set_cursor(path);
            return true;
        }
        return false;
    };

    auto selectScript2 = [&]() {
        for (auto child : store->children()) {
            if (selectScript(child)) return;
            for (auto child2 : child->children()) {
                if (selectScript(child2)) return;
            }
        }
    };

    // select script in tree view
    selectScript2();

    // set focus on editor
    editor->grabFocus();
    editor->setCursor(line, column);
}

void VRGuiScripts::getLineFocus(int& line, int& column) { editor->getCursor(line, column); }

void VRGuiScripts::on_search_link_clicked(searchResult res, string s) {
    focusScript(res.scriptName, res.line, res.column);
}

void VRGuiScripts::on_find_diag_find_clicked() {
    bool sa = getCheckButtonState("checkbutton38");
    //bool rep = getCheckButtonState("checkbutton12");
    string search = getTextEntry("entry10");
    if (search == "") return;
    hideDialog("find_dialog");

    VRScriptPtr s = getSelectedScript();
    if (!sa && s == 0) return;

    auto scene = VRScene::getCurrent();
    if (scene == 0) return;

    vector<VRScriptPtr> results;
    if (!sa) results = scene->searchScript(search, s);
    else results = scene->searchScript(search);

    auto print = [&]( string m, string style = "", shared_ptr< VRFunction<string> > link = 0 ) {
        VRGuiManager::get()->getConsole( "Search results" )->write( m, style, link );
    };

    VRGuiManager::get()->getConsole( "Search results" )->addStyle( "blueLink", "#3355ff", "#ffffff", false, true, true );

    // result output
    print( "Results, line-position, for search of '" + search + "':\n");
    for (auto r : results) {
        print( r->getName()+":" );
        for (auto r2 : r->getSearch().result) {
            for (auto p : r2.second) {
                print( " " );
                stringstream out;
                out << r2.first << "-" << p;
                searchResult sRes(r->getName(), r2.first, p);
                auto fkt = VRFunction<string>::create("search_link", boost::bind(&VRGuiScripts::on_search_link_clicked, this, sRes, _1) );
                print( out.str(), "blueLink", fkt );
            }
        }
        print( "\n" );
    }
    print( "\n" );
    updateList();
}

void VRGuiScripts::on_toggle_find_replace() {
    //setEntrySensitivity("entry11", getCheckButtonState("checkbutton12") );
    setEntrySensitivity("entry11", false);
    setTextEntry("entry11", "Coming soon!");
}

// config stuff

void VRGuiScripts::on_change_script_type() {
    if(!trigger_cbs) return;
    VRScriptPtr script = getSelectedScript();
    string t = getComboboxText("combobox1");
    script->setType(t);
    on_select_script();
    on_save_clicked();
}

void VRGuiScripts::on_change_group() {
    if(!trigger_cbs) return;
    VRScriptPtr script = getSelectedScript();
    script->setGroup( getComboboxText("combobox10") );
    on_select_script();
    on_save_clicked();
    updateList();
}

void VRGuiScripts::on_change_server() {
    if(!trigger_cbs) return;
    VRScriptPtr script = getSelectedScript();
    script->setHTMLHost( getComboboxText("combobox24") );
    on_select_script();
    on_save_clicked();
}


// --------------------------
// ---------Main-------------
// --------------------------

void VRGuiScripts::on_scene_changed() {
    groups.clear();
}

void VRGuiScripts::update() {
    auto scene = VRScene::getCurrent();
    if (scene == 0) return;
    for (auto r : scriptRows) setScriptListRow(r.second, r.first.lock(), true);
}

void VRGuiScripts::updateList() {
    auto scene = VRScene::getCurrent();
    if (scene == 0) return;

    // update script list
    Glib::RefPtr<Gtk::TreeStore> store = Glib::RefPtr<Gtk::TreeStore>::cast_static(VRGuiBuilder()->get_object("script_tree"));
    store->clear();

    auto oldpages = pages;
    pages.clear();

    map<string, pair<int,Gtk::TreeIter>> grpIter;
    auto addGroupRow = [&](group& g) {
        auto i = store->append();
        grpIter[g.name] = pair<int,Gtk::TreeIter>(g.ID,i);
        setGroupListRow(i, g);
        g.scripts.clear();
    };

    for (auto& g : groups) addGroupRow(g.second); // add all groups to list

    for (auto script : scene->getScripts()) { // check for new groups
        string grp = script.second->getGroup();
        if (grp != "" && grp != "no group" && !grpIter.count(grp)) {
            group g;
            groups[g.ID] = g;
            groups[g.ID].name = grp;
            addGroupRow(groups[g.ID]);
        }
    }

    scriptRows.clear();
    for (auto script : scene->getScripts()) {
        auto s = script.second;
        auto k = s.get();
        Gtk::TreeIter itr;
        string grp = s->getGroup();
        if (!grpIter.count(grp)) itr = store->append();
        else {
            auto& g = groups[grpIter[grp].first];
            g.scripts.push_back(s);
            itr = store->append(grpIter[grp].second->children());
        }
        scriptRows.push_back( pair<VRScriptPtr, Gtk::TreeIter>(s,itr) );
        setScriptListRow(itr, s);
        if (oldpages.count(k)) pages[k] = oldpages[k];
        else pages[k] = page();
    }
    on_select_script();
}

bool VRGuiScripts_on_editor_select(GtkWidget* widget, GdkEvent* event, VRGuiScripts* self) {
    GdkEventButton* event_btn = (GdkEventButton*)event;

    if (event->type == GDK_BUTTON_RELEASE && event_btn->button == 1) {
        auto editor = GTK_TEXT_VIEW(widget);
        auto buffer = gtk_text_view_get_buffer(editor);

        GtkTextIter A, B;
        gchar* selection = 0;
        if ( gtk_text_buffer_get_selection_bounds(buffer, &A, &B) ) {
            selection = gtk_text_buffer_get_text(buffer, &A, &B, true);
        }
        self->getEditor()->highlightStrings(selection?selection:"", "asSelected");
        return false;
    }

    if (event->type == GDK_KEY_RELEASE || event->type == GDK_BUTTON_RELEASE) { // remove highlights on any key or button
        self->getEditor()->highlightStrings("", "asSelected");
        return false;
    }
    return false;
}

VRGuiScripts::VRGuiScripts() {
    setToolButtonCallback("toolbutton6", sigc::mem_fun(*this, &VRGuiScripts::on_new_clicked) );
    setToolButtonCallback("toolbutton7", sigc::mem_fun(*this, &VRGuiScripts::on_save_clicked) );
    setToolButtonCallback("toolbutton8", sigc::mem_fun(*this, &VRGuiScripts::on_exec_clicked) );
    setToolButtonCallback("toolbutton9", sigc::mem_fun(*this, &VRGuiScripts::on_del_clicked) );
    setToolButtonCallback("toolbutton16", sigc::mem_fun(*this, &VRGuiScripts::on_help_clicked) );
    setToolButtonCallback("toolbutton20", sigc::mem_fun(*this, &VRGuiScripts::on_addSep_clicked) );
    setToolButtonCallback("toolbutton22", sigc::mem_fun(*this, &VRGuiScripts::on_import_clicked) );
    setToolButtonCallback("toolbutton23", sigc::mem_fun(*this, &VRGuiScripts::on_find_clicked) );
    setToolButtonCallback("toggletoolbutton1", sigc::mem_fun(*this, &VRGuiScripts::on_perf_toggled) );
    setToolButtonCallback("toggletoolbutton2", sigc::mem_fun(*this, &VRGuiScripts::on_pause_toggled) );

    setButtonCallback("button12", sigc::mem_fun(*this, &VRGuiScripts::on_argadd_clicked) );
    setButtonCallback("button13", sigc::mem_fun(*this, &VRGuiScripts::on_argrem_clicked) );
    setButtonCallback("button23", sigc::mem_fun(*this, &VRGuiScripts::on_trigadd_clicked) );
    setButtonCallback("button24", sigc::mem_fun(*this, &VRGuiScripts::on_trigrem_clicked) );
    setButtonCallback("button16", sigc::mem_fun(*this, &VRGuiScripts::on_help_close_clicked) );
    setButtonCallback("button28", sigc::mem_fun(*this, &VRGuiScripts::on_find_diag_cancel_clicked) );
    setButtonCallback("button29", sigc::mem_fun(*this, &VRGuiScripts::on_find_diag_find_clicked) );

    setCheckButtonCallback("checkbutton12", sigc::mem_fun(*this, &VRGuiScripts::on_toggle_find_replace) );

    setComboboxCallback("combobox1", sigc::mem_fun(*this, &VRGuiScripts::on_change_script_type) );
    setComboboxCallback("combobox10", sigc::mem_fun(*this, &VRGuiScripts::on_change_group) );
    setComboboxCallback("combobox24", sigc::mem_fun(*this, &VRGuiScripts::on_change_server) );

    // trigger tree_view
    Glib::RefPtr<Gtk::TreeView> tree_view  = Glib::RefPtr<Gtk::TreeView>::cast_static(VRGuiBuilder()->get_object("treeview14"));
    tree_view->add_events((Gdk::EventMask)GDK_KEY_PRESS_MASK);
    tree_view->signal_event().connect(sigc::mem_fun(*this, &VRGuiScripts::on_any_event));
    tree_view->signal_key_press_event().connect(sigc::mem_fun(*this, &VRGuiScripts::on_any_key_event));

    setTreeviewSelectCallback("treeview14", sigc::mem_fun(*this, &VRGuiScripts::on_select_trigger) );
    setTreeviewSelectCallback("treeview5", sigc::mem_fun(*this, &VRGuiScripts::on_select_script) );
    setTreeviewSelectCallback("treeview3", sigc::mem_fun(*this, &VRGuiScripts::on_select_help) );

    editor = shared_ptr<VRGuiEditor>( new VRGuiEditor("scrolledwindow4") );
    editor->addKeyBinding("find", VRUpdateCb::create("findCb", boost::bind(&VRGuiScripts::on_find_clicked, this)));
    editor->addKeyBinding("save", VRUpdateCb::create("saveCb", boost::bind(&VRGuiScripts::on_save_clicked, this)));
    editor->addKeyBinding("exec", VRUpdateCb::create("execCb", boost::bind(&VRGuiScripts::on_exec_clicked, this)));
    g_signal_connect(editor->getSourceBuffer(), "changed", G_CALLBACK(VRGuiScripts_on_script_changed), this);

    setEntryCallback("entry10", sigc::mem_fun(*this, &VRGuiScripts::on_find_diag_find_clicked), false, false);

    setCellRendererCallback("cellrenderertext13", sigc::mem_fun(*this, &VRGuiScripts::on_name_edited) );
    setCellRendererCallback("cellrenderertext2", sigc::mem_fun(*this, &VRGuiScripts::on_argname_edited) );
    setCellRendererCallback("cellrenderertext14", sigc::mem_fun(*this, &VRGuiScripts::on_argval_edited) );
    setCellRendererCallback("cellrenderertext16", sigc::mem_fun(*this, &VRGuiScripts::on_trigparam_edited) );
    setCellRendererCallback("cellrenderertext41", sigc::mem_fun(*this, &VRGuiScripts::on_trigkey_edited) );

    VRGuiScripts_ArgsModelColumns acols;
    VRGuiScripts_TrigsModelColumns tcols;
    setCellRendererCombo("treeviewcolumn16", "arg_types", acols.type, sigc::mem_fun(*this, &VRGuiScripts::on_argtype_edited) );
    setCellRendererCombo("treeviewcolumn27", "ScriptTrigger", tcols.trigs, sigc::mem_fun(*this, &VRGuiScripts::on_trigger_edited) );
    setCellRendererCombo("treeviewcolumn28", "ScriptTriggerDevices", tcols.devs, sigc::mem_fun(*this, &VRGuiScripts::on_trigdev_edited) );
    setCellRendererCombo("treeviewcolumn30", "ScriptTriggerStates", tcols.state, sigc::mem_fun(*this, &VRGuiScripts::on_trigstate_edited) );

    // fill combolists
    const char *arg_types[] = {"int", "float", "str", "VRPyObjectType", "VRPyTransformType", "VRPyGeometryType", "VRPyLightType", "VRPyLodType", "VRPyDeviceType", "VRPyMouseType", "VRPyHapticType", "VRPySocketType", "VRPyLeapFrameType"};
    const char *trigger_types[] = {"none", "on_scene_load", "on_scene_close", "on_scene_import", "on_timeout", "on_device", "on_socket"};
    const char *device_types[] = {"mouse", "multitouch", "keyboard", "flystick", "haptic", "server1", "leap", "vrpn_device"}; // TODO: get from a list in devicemanager or something
    const char *trigger_states[] = {"Pressed", "Released", "Drag", "Drop", "To edge", "From edge"};
    const char *script_types[] = {"Python", "GLSL", "HTML"};
    fillStringListstore("arg_types", vector<string>(arg_types, end(arg_types)) );
    fillStringListstore("ScriptTrigger", vector<string>(trigger_types, end(trigger_types)) );
    fillStringListstore("ScriptTriggerDevices", vector<string>(device_types, end(device_types)) );
    fillStringListstore("ScriptTriggerStates", vector<string>(trigger_states, end(trigger_states)) );
    fillStringListstore("liststore6", vector<string>(script_types, end(script_types)) );

    setToolButtonSensitivity("toolbutton7", false);
    setToolButtonSensitivity("toolbutton8", false);
    setToolButtonSensitivity("toolbutton9", false);

    // update the list each frame to update the execution time
    updatePtr = VRUpdateCb::create("scripts_gui_update",  boost::bind(&VRGuiScripts::update, this) );
    VRSceneManager::get()->addUpdateFkt(updatePtr, 100);

    sceneChangedCb = VRFunction<VRDeviceWeakPtr>::create("GUI_sceneChanged", boost::bind(&VRGuiScripts::on_scene_changed, this) );
    VRGuiSignals::get()->getSignal("scene_changed")->add( sceneChangedCb );

    // init scriptImportWidget
    scriptImportWidget = manage( new Table() );
    ScrolledWindow* sw1 = manage( new ScrolledWindow() );
    ScrolledWindow* sw2 = manage( new ScrolledWindow() );
    import_treeview1 = manage( new TreeView() );
    import_treeview2 = manage( new TreeView() );

    VRGuiScripts_ModelColumns columns;
    import_liststore1 = ListStore::create( columns );
    import_liststore2 = ListStore::create( columns );

    scriptImportWidget->attach(*sw1, 0,1,0,1);
    scriptImportWidget->attach(*sw2, 0,1,1,2);
    sw1->add(*import_treeview1); sw2->add(*import_treeview2);

    import_treeview1->set_model(import_liststore1);
    import_treeview2->set_model(import_liststore2);
    import_treeview1->append_column("File scripts", columns.script);
    import_treeview2->append_column("Project scripts", columns.script);
    import_treeview1->signal_cursor_changed().connect( sigc::mem_fun(*this, &VRGuiScripts::on_diag_import_select_1) );
    import_treeview2->signal_cursor_changed().connect( sigc::mem_fun(*this, &VRGuiScripts::on_diag_import_select_2) );

    // documentation widget
    setEntryCallback("entry25", sigc::mem_fun(*this, &VRGuiScripts::on_doc_filter_edited), true);
}

OSG_END_NAMESPACE;
