#include "GOAPGraphEditor.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void GOAPGraphEditor::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_goap_asset", "asset"), &GOAPGraphEditor::set_goap_asset);
    ClassDB::bind_method(D_METHOD("get_goap_asset"), &GOAPGraphEditor::get_goap_asset);
    ClassDB::bind_method(D_METHOD("add_action_node", "name"), &GOAPGraphEditor::add_action_node);
    ClassDB::bind_method(D_METHOD("add_goal_node", "name"), &GOAPGraphEditor::add_goal_node);
    ClassDB::bind_method(D_METHOD("remove_node", "id"), &GOAPGraphEditor::remove_node);
    ClassDB::bind_method(D_METHOD("update_debug_view", "instance"), &GOAPGraphEditor::update_debug_view);

    ADD_SIGNAL(MethodInfo("node_selected", PropertyInfo(Variant::OBJECT, "node")));
    ADD_SIGNAL(MethodInfo("node_deleted", PropertyInfo(Variant::INT, "id")));
}

GOAPGraphEditor::GOAPGraphEditor()
{
    context_menu = memnew(PopupMenu);
    add_child(context_menu);
    context_menu->add_item("Add Action", 0);
    context_menu->add_item("Add Goal", 1);
    context_menu->connect("id_pressed", callable_mp(this, &GOAPGraphEditor::_on_context_menu_id_pressed));

    next_node_position = Vector2(50, 50);

    connect("node_selected", callable_mp(this, &GOAPGraphEditor::_on_node_selected));
    connect("node_deleted", callable_mp(this, &GOAPGraphEditor::_on_node_deleted));
}

GOAPGraphEditor::~GOAPGraphEditor()
{
    if (context_menu)
    {
        memdelete(context_menu);
        context_menu = nullptr;
    }
}

void GOAPGraphEditor::set_goap_asset(const Ref<GOAPAsset> &p_asset)
{
    goap_asset = p_asset;
    update_debug_view(nullptr);
}

Ref<GOAPAsset> GOAPGraphEditor::get_goap_asset() const { return goap_asset; }

void GOAPGraphEditor::add_action_node(const String &p_name)
{
    // Ref<GraphNode> node;
    // node.instantiate();
    // node->set_title("Action: " + p_name);
    // node->set_position_offset(next_node_position);
    // next_node_position += Vector2(20, 20);
    // add_child(node.ptr());
}

void GOAPGraphEditor::add_goal_node(const String &p_name)
{
    // Ref<GraphNode> node;
    // node.instantiate();
    // node->set_title("Goal: " + p_name);
    // node->set_position_offset(next_node_position);
    // next_node_position += Vector2(20, 20);
    // add_child(node.ptr());
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

    // Clear existing debug info
    for (int i = 0; i < get_child_count(); i++)
    {
        Node *child = get_child(i);
        if (child->is_class("GraphNode"))
        {
            GraphNode *node = Object::cast_to<GraphNode>(child);
            node->set_slot(0, false, 0, Color(), true, 0, Color());
        }
    }

    // Show current plan connections
    // Basic debug visualization
    if (goap_asset->has_method("get_current_plan"))
    {
        Array plan = goap_asset->call("get_current_plan");
        for (int i = 0; i < plan.size() - 1; i++)
        {
            Node *from = find_child(String::num_int64(plan[i]), true);
            Node *to = find_child(String::num_int64(plan[i + 1]), true);

            if (from && to && from->is_class("GraphNode") && to->is_class("GraphNode"))
            {
                GraphNode *from_node = Object::cast_to<GraphNode>(from);
                GraphNode *to_node = Object::cast_to<GraphNode>(to);

                from_node->set_slot(0, false, 0, Color(), true, 0, Color(0.2, 0.8, 0.2));
                to_node->set_slot(0, true, 0, Color(0.2, 0.8, 0.2), false, 0, Color());

                connect_node(from_node->get_name(), 0, to_node->get_name(), 0);
            }
        }
    }
}

void GOAPGraphEditor::_on_node_selected(Node *p_node) { emit_signal("node_selected", p_node); }

void GOAPGraphEditor::_on_node_deleted(int p_id) { emit_signal("node_deleted", p_id); }

void GOAPGraphEditor::_gui_input(const Ref<InputEvent> &p_event)
{
    Ref<InputEventMouseButton> mb = p_event;
    if (mb.is_valid() && mb->get_button_index() == MOUSE_BUTTON_RIGHT && mb->is_pressed())
    {
        Vector2 local_pos = get_local_mouse_position();
        next_node_position = local_pos;

        context_menu->set_position(get_screen_position() + mb->get_position());
        context_menu->popup();
        accept_event();
    }
}

void GOAPGraphEditor::_on_context_menu_id_pressed(int p_id)
{
    switch (p_id)
    {
    case 0:
        add_action_node("NewAction");
        break;
    case 1:
        add_goal_node("NewGoal");
        break;
    }
}