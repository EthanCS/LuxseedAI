#ifndef GOAP_GRAPH_EDITOR_H
#define GOAP_GRAPH_EDITOR_H

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/graph_edit.hpp>
#include <godot_cpp/classes/graph_node.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include "../GOAPAsset.h"
#include "../GOAPPlanner.h"

using namespace godot;

class GOAPPlanner;

class GOAPGraphEditor : public GraphEdit
{
    GDCLASS(GOAPGraphEditor, GraphEdit)

private:
    Ref<GOAPAsset> goap_asset;
    Vector2 next_node_position;
    PopupMenu *context_menu;

protected:
    static void _bind_methods();

    void _on_node_selected(Node *p_node);
    void _on_node_deleted(int p_id);
    void _on_context_menu_id_pressed(int p_id);

public:
    virtual void _gui_input(const Ref<InputEvent> &p_event) override;

public:
    GOAPGraphEditor();
    ~GOAPGraphEditor();

    void set_goap_asset(const Ref<GOAPAsset> &p_asset);
    Ref<GOAPAsset> get_goap_asset() const;

    void add_action_node(const String &p_name);
    void add_goal_node(const String &p_name);
    void remove_node(int p_id);

    void update_debug_view(const GOAPPlanner *p_planner);
};

#endif // GOAP_GRAPH_EDITOR_H