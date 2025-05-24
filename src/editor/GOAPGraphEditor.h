#ifndef GOAP_GRAPH_EDITOR_H
#define GOAP_GRAPH_EDITOR_H

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/graph_edit.hpp>
#include <godot_cpp/classes/graph_node.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/h_box_container.hpp>

#include "../GOAPAsset.h"
#include "../GOAPPlanner.h"

using namespace godot;

class GOAPPlanner;

class GOAPGraphNode : public GraphNode
{
    GDCLASS(GOAPGraphNode, GraphNode)

public:
    enum class NodeType : unsigned int
    {
        UNKNOWN,
        GOAL,
        ACTION
    };

private:
    Ref<GOAPGoalAsset> goal_asset;
    Ref<GOAPActionAsset> action_asset;
    NodeType node_type;

protected:
    void _notification(int p_what);
    static void _bind_methods();
    void _on_asset_name_changed();
    static StyleBoxFlat *get_titlebar_stylebox_flat(NodeType p_node_type);

public:
    GOAPGraphNode();

    NodeType get_node_type() const noexcept { return node_type; }

    godot::Object *get_asset_object() const noexcept
    {
        if (node_type == NodeType::GOAL && goal_asset.is_valid())
            return goal_asset.ptr();
        else if (node_type == NodeType::ACTION && action_asset.is_valid())
            return action_asset.ptr();
        return nullptr;
    }

    void set_goal_asset(const Ref<GOAPGoalAsset> &p_asset) noexcept
    {
        goal_asset = p_asset;
        node_type = NodeType::GOAL;
    }

    void set_action_asset(const Ref<GOAPActionAsset> &p_asset) noexcept
    {
        action_asset = p_asset;
        node_type = NodeType::ACTION;
    }

    friend class GOAPGraphEditor;
};

class GOAPGraphEditor : public GraphEdit
{
    GDCLASS(GOAPGraphEditor, GraphEdit)

private:
    Ref<GOAPAsset> goap_asset;
    PopupMenu *context_menu;
    Button *new_button;
    HBoxContainer *debug_selector_container;
    Label *debug_label;
    OptionButton *debug_selector;
    Vector2 next_node_position;
    EditorInterface *editor_interface;

protected:
    static void _bind_methods();

    void _on_node_selected(Node *p_node);
    void _on_node_deselected(Node *p_node);
    void _on_delete_nodes_request(Array node_names);

    void _on_node_deleted(int p_id);
    void _on_context_menu_id_pressed(int p_id);
    void _on_new_button_pressed();

    void _on_debug_instance_selected(int p_id);
    void _on_debug_instance_list_changed();

public:
    virtual void _gui_input(const Ref<InputEvent> &p_event) override;
    virtual PackedVector2Array _get_connection_line(const Vector2 &p_from_position,
                                                    const Vector2 &p_to_position) const override;

public:
    GOAPGraphEditor();
    ~GOAPGraphEditor();

    void set_goap_asset(const Ref<GOAPAsset> &p_asset);
    Ref<GOAPAsset> get_goap_asset() const { return goap_asset; }

    void add_action_node(Ref<GOAPActionAsset> p_action);
    void add_goal_node(Ref<GOAPGoalAsset> p_goal);

    void update_debug_view(const GOAPPlanner *p_planner);
};

#endif // GOAP_GRAPH_EDITOR_H