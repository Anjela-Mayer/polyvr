#include "VRAppPanel.h"
#include "VRAppLauncher.h"

#include "core/utils/system/VRSystem.h"
#include "core/scene/VRSceneManager.h"

#include <gtk/gtk.h>

using namespace OSG;

VRAppPanel::VRAppPanel(string name, _GtkWidget* t) : table(t) {
    setNameSpace("__system_apps__");
    setName(name);
}

VRAppPanel::~VRAppPanel() {}

VRAppPanelPtr VRAppPanel::create(string name, _GtkWidget* table) { return VRAppPanelPtr( new VRAppPanel(name, table) ); }
VRAppPanelPtr VRAppPanel::ptr() { return shared_from_this(); }

VRAppLauncherPtr VRAppPanel::addLauncher(string path, string timestamp, VRGuiContextMenu* menu, VRAppManager* mgr, bool write_protected, bool favorite, string table) {
    if (!exists(path)) return 0;
    if (apps.count(path)) return apps[path];
    auto app = VRAppLauncher::create(ptr());
    app->path = path;
    app->lastStarted = timestamp;
    string filename = getFileName(path);
    string foldername = getFolderName(path);
    app->pxm_path = foldername + "/.local_" + filename.substr(0,filename.size()-4) + "/snapshot.png";
    app->write_protected = write_protected;
    app->favorite = favorite;
    app->table = table;
    apps[path] = app;
    app->setup(menu, mgr);
    return app;
}

int VRAppPanel::getSize() { return apps.size(); }
_GtkWidget* VRAppPanel::getTable() { return table; }

void VRAppPanel::fillTable(string t, int& i) {
    int x,y;
    GtkAttachOptions optsH = GtkAttachOptions(GTK_FILL|GTK_EXPAND);
    GtkAttachOptions optsV = GTK_SHRINK;
    //GtkAttachOptions optsV = GtkAttachOptions(0);

    for (auto d : apps) {
        if (d.second->table != t) continue;
        if (d.second->widget == 0) continue;

        GtkWidget* w = (GtkWidget*)d.second->widget;
        x = i%2;
        y = i/2;
#if GTK_MAJOR_VERSION == 2
        gtk_table_attach(GTK_TABLE(table), w, x, x+1, y, y+1, optsH, optsV, 10, 10);
#else
        gtk_grid_attach(GTK_GRID(table), w, x, y, 1, 1);
#endif
        i++;
    }
    gtk_widget_show(table);
}

void VRAppPanel::clearTable(string t) {
    for (auto d : apps) {
        if (d.second->table != t) continue;

        GtkWidget* w = (GtkWidget*)d.second->widget;
        if (w == 0) continue;
        gtk_container_remove((GtkContainer*)table, w);
    }
}

void VRAppPanel::setGuiState(VRAppLauncherPtr e, bool running, bool noLauncherScene) {
    for (auto i : apps) {
        if (noLauncherScene) i.second->setState(1); // disable all launchers
        else if (running) {
            if (i.second == e) i.second->setState(2); // running scene
            else               i.second->setState(1); // disable other scenes
        } else i.second->setState(0); // all launchers standby
    }
}

void VRAppPanel::remLauncher(string path) { apps.erase(path); }
VRAppLauncherPtr VRAppPanel::getLauncher(string path) { return apps.count(path) ? apps[path] : 0; }

map<string, VRAppLauncherPtr> VRAppPanel::getLaunchers() { return apps; }
