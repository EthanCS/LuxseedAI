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

class GOAPGoalNode : public GraphNode
{
    GDCLASS(GOAPGoalNode, GraphNode)

private:
    Ref<GOAPGoalAsset> goal_asset;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    GOAPGoalNode();
    void set_goal_asset(const Ref<GOAPGoalAsset> &p_asset) noexcept { goal_asset = p_asset; }
    Ref<GOAPGoalAsset> get_goal_asset() const noexcept { return goal_asset; }
};

class GOAPActionNode : public GraphNode
{
    GDCLASS(GOAPActionNode, GraphNode)

private:
    Ref<GOAPActionAsset> action_asset;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    GOAPActionNode();
    void set_action_asset(const Ref<GOAPActionAsset> &p_asset) noexcept { action_asset = p_asset; }
    Ref<GOAPActionAsset> get_action_asset() const noexcept { return action_asset; }
};

class GOAPGraphEditor : public GraphEdit
{
    GDCLASS(GOAPGraphEditor, GraphEdit)

private:
    Ref<GOAPAsset> goap_asset;
    PopupMenu *context_menu;
    Vector2 next_node_position;

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

    void add_action_node(Ref<GOAPActionAsset> p_action);
    void add_goal_node(Ref<GOAPGoalAsset> p_goal);

    void remove_node(int p_id);

    void update_debug_view(const GOAPPlanner *p_planner);
};

#endif // GOAP_GRAPH_EDITOR_H