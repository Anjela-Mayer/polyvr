#ifndef VRGUIFILE_H_INCLUDED
#define VRGUIFILE_H_INCLUDED

#include <string>
#include <vector>
#include <functional>

struct _GtkFileChooserDialog;
struct _GtkTable;
struct _GtkCheckButton;
struct _GtkEntry;
struct _GtkComboBox;
union _GdkEvent;

using namespace std;

class VRGuiFile {
    private:
        static _GtkFileChooserDialog* dialog;
        static _GtkTable* addon;
        static _GtkTable* geoImportWidget;
        static _GtkTable* saveasWidget;
        static function<void()> sigApply;
        static function<void()> sigClose;
        static function<void()> sigSelect;
        static bool cache_override;
        static float scale;
        static string preset;
        static void init();

        static void on_toggle_cache_override(_GtkCheckButton* b);
        static void on_edit_import_scale(_GtkEntry* e);
        static void on_change_preset(_GtkComboBox* b);

    public:
        static void open(string button, int action, string title);
        static void close();
        static void apply();
        static bool keyApply(_GdkEvent*);
        static void select();
        static string getPath();
        static string getRelativePath_toOrigin();
        static string getRelativePath_toWorkdir();

        static void gotoPath(string path);
        static void setFile(string file);
        static void addFilter(string name, int N, ...);
        static void clearFilter();

        static void setCallbacks(function<void()> sa = function<void()>(), function<void()> sc = function<void()>(), function<void()> ss = function<void()>());
        static void setWidget(_GtkTable* table, bool expand = false, bool fill = false);
        static void setGeoLoadWidget();
        static void setSaveasWidget(function<void(_GtkCheckButton*)> sig);

        static bool exists(string path);
        static bool isDir(string path);
        static bool isFile(string path);
        static vector<string> listDir(string dir);

        static float getScale() { return scale; }
        static string getPreset() { return preset; }
};

#endif // VRGUIFILE_H_INCLUDED
