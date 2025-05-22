#include "GOAPEditorPlugin.h"
#include "../GOAPAsset.h"
#include "GOAPGraphEditor.h"
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_interface.hpp>

void GOAPEditorPlugin::_bind_methods()
{
    // No methods need to be bound for now
}

GOAPEditorPlugin::GOAPEditorPlugin()
{
    graph_editor = memnew(GOAPGraphEditor);
    graph_editor->set_custom_minimum_size(Size2(0, 300));
    add_control_to_bottom_panel(graph_editor, "GOAP Editor");
}

GOAPEditorPlugin::~GOAPEditorPlugin()
{
    if (graph_editor)
    {
        memdelete(graph_editor);
    }
}

bool GOAPEditorPlugin::_handles(Object *p_object) const { return Object::cast_to<GOAPAsset>(p_object) != nullptr; }

void GOAPEditorPlugin::_edit(Object *p_object)
{
    current_asset = Ref<GOAPAsset>(Object::cast_to<GOAPAsset>(p_object));
    if (current_asset.is_valid())
    {
        graph_editor->set_goap_asset(current_asset);
    }
}

void GOAPEditorPlugin::_make_visible(bool p_visible)
{
    if (p_visible)
    {
        make_bottom_panel_item_visible(graph_editor);
    }
}