#ifndef GOAP_EDITOR_PLUGIN_H
#define GOAP_EDITOR_PLUGIN_H

#include <godot_cpp/classes/editor_plugin.hpp>
#include "GOAPGraphEditor.h"

using namespace godot;

class GOAPEditorPlugin : public EditorPlugin {
    GDCLASS(GOAPEditorPlugin, EditorPlugin)

private:
    Ref<GOAPAsset> current_asset;
    GOAPGraphEditor *graph_editor;

protected:
    static void _bind_methods();

public:
    GOAPEditorPlugin();
    ~GOAPEditorPlugin();

    virtual bool _handles(Object *p_object) const override;
    virtual void _edit(Object *p_object) override;
    virtual void _make_visible(bool p_visible) override;
};

#endif // GOAP_EDITOR_PLUGIN_H