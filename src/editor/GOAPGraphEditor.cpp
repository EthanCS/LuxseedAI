#include "GOAPGraphEditor.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/graph_node.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/theme.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

//////////////////////////////////////////////////////////////////////////////////////
// GOAPGoalNode
//////////////////////////////////////////////////////////////////////////////////////
GOAPGoalNode::GOAPGoalNode() : goal_asset(nullptr) { set_name("GOAPGoalNode"); }

void GOAPGoalNode::_notification(int p_what)
{
    if (p_what == NOTIFICATION_READY)
    {
        // Make node larger
        set_custom_minimum_size(Vector2(200, 150));

        // Set green background
        Ref<StyleBoxFlat> sb = get_theme_stylebox("panel")->duplicate();
        sb->set_bg_color(Color(0.2, 0.8, 0.2));
        add_theme_stylebox_override("panel", sb);

        // Make title bigger
        add_theme_font_size_override("title_font_size", 20);
    }
}

void GOAPGoalNode::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("_on_asset_name_changed"), &GOAPGoalNode::_on_asset_name_changed);
}

void GOAPGoalNode::_on_asset_name_changed()
{
    if (goal_asset.is_valid())
    {
        set_title(goal_asset->get_name());
        set_name(goal_asset->get_name());
    }
}

//////////////////////////////////////////////////////////////////////////////////////
// GOAPActionNode
//////////////////////////////////////////////////////////////////////////////////////
GOAPActionNode::GOAPActionNode() : action_asset(nullptr) { set_name("GOAPActionNode"); }

void GOAPActionNode::_notification(int p_what)
{
    if (p_what == NOTIFICATION_READY)
    {
        // Make node larger
        set_custom_minimum_size(Vector2(200, 150));

        // Make title bigger
        add_theme_font_size_override("title_font_size", 20);
    }
}

void GOAPActionNode::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("_on_asset_name_changed"), &GOAPActionNode::_on_asset_name_changed);
}

void GOAPActionNode::_on_asset_name_changed()
{
    if (action_asset.is_valid())
    {
        set_title(action_asset->get_name());
        set_name(action_asset->get_name());
    }
}

//////////////////////////////////////////////////////////////////////////////////////
// GOAPGraphEditor
//////////////////////////////////////////////////////////////////////////////////////
GOAPGraphEditor::GOAPGraphEditor() : editor_interface(nullptr)
{
    context_menu = memnew(PopupMenu);

    if (Engine::get_singleton()->is_editor_hint())
        editor_interface = EditorInterface::get_singleton();

    add_child(context_menu);
    context_menu->add_item("Add Action", 0);
    context_menu->add_item("Add Goal", 1);
    context_menu->connect("id_pressed", callable_mp(this, &GOAPGraphEditor::_on_context_menu_id_pressed));

    connect("node_selected", callable_mp(this, &GOAPGraphEditor::_on_node_selected));
    connect("node_deselected", callable_mp(this, &GOAPGraphEditor::_on_node_deselected));
    connect("delete_nodes_request", callable_mp(this, &GOAPGraphEditor::_on_delete_nodes_request));
}

GOAPGraphEditor::~GOAPGraphEditor()
{
    if (context_menu)
    {
        memdelete(context_menu);
        context_menu = nullptr;
    }
}

void GOAPGraphEditor::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_goap_asset", "asset"), &GOAPGraphEditor::set_goap_asset);
    ClassDB::bind_method(D_METHOD("get_goap_asset"), &GOAPGraphEditor::get_goap_asset);
    ClassDB::bind_method(D_METHOD("add_action_node", "action"), &GOAPGraphEditor::add_action_node);
    ClassDB::bind_method(D_METHOD("add_goal_node", "goal"), &GOAPGraphEditor::add_goal_node);
    ClassDB::bind_method(D_METHOD("remove_node", "id"), &GOAPGraphEditor::remove_node);
    ClassDB::bind_method(D_METHOD("update_debug_view", "instance"), &GOAPGraphEditor::update_debug_view);
}

void GOAPGraphEditor::_on_node_selected(Node *node)
{
    if (node == nullptr)
        return;

    UtilityFunctions::print("Node selected: ", node->get_name());

    if (editor_interface && Engine::get_singleton()->is_editor_hint())
    {
        if (node->is_class("GOAPActionNode"))
        {
            GOAPActionNode *action_node = Object::cast_to<GOAPActionNode>(node);
            Ref<GOAPActionAsset> asset = action_node->get_action_asset();
            if (asset.is_valid())
            {
                editor_interface->inspect_object(asset.ptr());
            }
        }
        else if (node->is_class("GOAPGoalNode"))
        {
            GOAPGoalNode *goal_node = Object::cast_to<GOAPGoalNode>(node);
            Ref<GOAPGoalAsset> asset = goal_node->get_goal_asset();
            if (asset.is_valid())
            {
                editor_interface->inspect_object(asset.ptr());
            }
        }
    }
}

void GOAPGraphEditor::_on_node_deselected(Node *node)
{
    if (node == nullptr)
        return;

    UtilityFunctions::print("Node unselected: ", node->get_name());

    if (editor_interface && Engine::get_singleton()->is_editor_hint())
    {
        editor_interface->inspect_object(goap_asset.ptr());
    }
}

void GOAPGraphEditor::_on_delete_nodes_request(Array node_names)
{
    for (int i = 0; i < node_names.size(); i++)
    {
        UtilityFunctions::print("Node delete request: ", node_names[i]);
    }
}

void GOAPGraphEditor::set_goap_asset(const Ref<GOAPAsset> &p_asset)
{
    if (goap_asset == p_asset)
        return;

    print_line("GOAPGraphEditor::set_goap_asset " + p_asset->get_path());
    goap_asset = p_asset;

    // Clear nodes
    for (int i = get_child_count() - 1; i >= 0; i--)
    {
        Node *child = get_child(i);
        if (child->is_class("GOAPActionNode") || child->is_class("GOAPGoalNode"))
        {
            remove_child(child);
            memdelete(child);
        }
    }

    // Add action nodes
    TypedArray<GOAPActionAsset> actions = p_asset->get_action_assets();
    for (int i = 0; i < actions.size(); i++)
    {
        add_action_node(actions[i]);
    }

    // Add goal nodes
    TypedArray<GOAPGoalAsset> goals = p_asset->get_goal_assets();
    for (int i = 0; i < goals.size(); i++)
    {
        add_goal_node(goals[i]);
    }

    update_debug_view(nullptr);
}

void GOAPGraphEditor::add_action_node(Ref<GOAPActionAsset> p_action)
{
    GOAPActionNode *node = memnew(GOAPActionNode);
    node->set_title(p_action->get_name());
    node->set_position_offset(p_action->get_editor_position());
    node->set_name(p_action->get_name());
    node->set_action_asset(p_action);
    if (p_action.is_valid())
    {
        p_action->connect("name_changed", callable_mp(node, &GOAPActionNode::_on_asset_name_changed));
    }
    add_child(node);
}

void GOAPGraphEditor::add_goal_node(Ref<GOAPGoalAsset> p_goal)
{
    GOAPGoalNode *node = memnew(GOAPGoalNode);
    node->set_title(p_goal->get_name());
    node->set_position_offset(p_goal->get_editor_position());
    node->set_name(p_goal->get_name());
    node->set_goal_asset(p_goal);
    if (p_goal.is_valid())
    {
        p_goal->connect("name_changed", callable_mp(node, &GOAPGoalNode::_on_asset_name_changed));
    }
    add_child(node);
}

void GOAPGraphEditor::remove_node(int p_id)
{
    Node *node = find_child(String::num_int64(p_id), true);
    if (!node)
        return;

    // Disconnect all signals first
    node->disconnect("close_request", callable_mp(this, &GOAPGraphEditor::_on_node_deleted));

    // Remove and delete the node
    remove_child(node);
    if (godot::Engine::get_singleton()->is_editor_hint())
    {
        memdelete(node);
    }
}

void GOAPGraphEditor::update_debug_view(const GOAPPlanner *p_planner)
{
    if (!goap_asset.is_valid())
        return;

    if (p_planner == nullptr)
        return;

    // // Clear existing debug info
    // for (int i = 0; i < get_child_count(); i++)
    // {
    //     Node *child = get_child(i);
    //     if (child->is_class("GraphNode"))
    //     {
    //         GraphNode *node = Object::cast_to<GraphNode>(child);
    //         node->set_slot(0, false, 0, Color(), true, 0, Color());
    //     }
    // }

    // // Show current plan connections
    // // Basic debug visualization
    // if (goap_asset->has_method("get_current_plan"))
    // {
    //     Array plan = goap_asset->call("get_current_plan");
    //     for (int i = 0; i < plan.size() - 1; i++)
    //     {
    //         Node *from = find_child(String::num_int64(plan[i]), true);
    //         Node *to = find_child(String::num_int64(plan[i + 1]), true);

    //         if (from && to && from->is_class("GraphNode") && to->is_class("GraphNode"))
    //         {
    //             GraphNode *from_node = Object::cast_to<GraphNode>(from);
    //             GraphNode *to_node = Object::cast_to<GraphNode>(to);

    //             from_node->set_slot(0, false, 0, Color(), true, 0, Color(0.2, 0.8, 0.2));
    //             to_node->set_slot(0, true, 0, Color(0.2, 0.8, 0.2), false, 0, Color());

    //             connect_node(from_node->get_name(), 0, to_node->get_name(), 0);
    //         }
    //     }
    // }
}

void GOAPGraphEditor::_on_node_deleted(int p_id) { emit_signal("node_deleted", p_id); }

Node *GOAPGraphEditor::get_node_at_position(const Vector2 &p_position) const
{
    for (int i = 0; i < get_child_count(); i++)
    {
        Node *child = get_child(i);
        if (child->is_class("GraphNode"))
        {
            GraphNode *node = Object::cast_to<GraphNode>(child);
            if (node->get_global_rect().has_point(p_position))
            {
                return node;
            }
        }
    }
    return nullptr;
}

void GOAPGraphEditor::_gui_input(const Ref<InputEvent> &p_event)
{
    Ref<InputEventMouseButton> mb = p_event;
    if (mb.is_valid() && mb->is_pressed())
    {
        if (mb->get_button_index() == MOUSE_BUTTON_LEFT)
        {
            // Handle left click selection
            Node *clicked = get_node_at_position(get_local_mouse_position());
            if (clicked && clicked->is_class("GraphNode"))
            {
                _on_node_selected(clicked);
            }
        }
        else if (mb->get_button_index() == MOUSE_BUTTON_RIGHT)
        {
            Vector2 local_pos = get_local_mouse_position();
            next_node_position = local_pos;

            context_menu->set_position(get_screen_position() + mb->get_position());
            context_menu->popup();
            accept_event();
        }
    }
}

void GOAPGraphEditor::_on_context_menu_id_pressed(int p_id)
{
    if (goap_asset == nullptr)
        return;

    switch (p_id)
    {
    case 0:
        add_action_node(goap_asset->new_action_asset("NewAction", next_node_position));
        break;
    case 1:
        add_goal_node(goap_asset->new_goal_asset("NewGoal", next_node_position));
        break;
    }
}