#include "GOAPGraphEditor.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/graph_node.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/theme.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

//////////////////////////////////////////////////////////////////////////////////////
// GOAPGraphNode
//////////////////////////////////////////////////////////////////////////////////////
GOAPGraphNode::GOAPGraphNode() : goal_asset(nullptr), action_asset(nullptr), node_type(NodeType::UNKNOWN) {}

void GOAPGraphNode::_notification(int p_what)
{
    if (p_what == NOTIFICATION_ENTER_TREE)
    {
        HBoxContainer *titlebar = get_titlebar_hbox();
        if (titlebar != nullptr)
        {
            this->add_theme_stylebox_override("titlebar", get_titlebar_stylebox_flat(node_type));
            this->add_theme_stylebox_override("titlebar_selected", get_titlebar_stylebox_flat(node_type));

            for (int i = 0; i < titlebar->get_child_count(); i++)
            {
                Node *child = titlebar->get_child(i);
                if (child->is_class("Label"))
                {
                    Label *label = Object::cast_to<Label>(child);
                    // label->add_theme_font_size_override("font_size", 30);
                    label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
                    label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
                }
            }
        }
    }
}

void GOAPGraphNode::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("_on_asset_name_changed"), &GOAPGraphNode::_on_asset_name_changed);
}

void GOAPGraphNode::_on_asset_name_changed()
{
    if (node_type == NodeType::GOAL)
    {
        if (goal_asset.is_valid())
        {
            Label *label = Object::cast_to<Label>(get_child(0));
            label->set_text(goal_asset->get_name());
            set_name(goal_asset->get_name());
        }
    }
    else if (node_type == NodeType::ACTION)
    {
        if (action_asset.is_valid())
        {
            Label *label = Object::cast_to<Label>(get_child(0));
            label->set_text(action_asset->get_name());
            set_name(action_asset->get_name());
        }
    }
}

StyleBoxFlat *GOAPGraphNode::get_titlebar_stylebox_flat(NodeType p_node_type)
{
    static StyleBoxFlat *stylebox_flat_goal = memnew(StyleBoxFlat);
    stylebox_flat_goal->set_bg_color(Color::html("#004d0d"));
    stylebox_flat_goal->set_content_margin_all(0);

    static StyleBoxFlat *stylebox_flat_action = memnew(StyleBoxFlat);
    stylebox_flat_action->set_bg_color(Color::html("#4d3800"));
    stylebox_flat_action->set_content_margin_all(0);

    if (p_node_type == NodeType::GOAL)
        return stylebox_flat_goal;
    else if (p_node_type == NodeType::ACTION)
        return stylebox_flat_action;

    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////
// GOAPGraphEditor
//////////////////////////////////////////////////////////////////////////////////////
GOAPGraphEditor::GOAPGraphEditor() : editor_interface(nullptr)
{
    if (Engine::get_singleton()->is_editor_hint())
        editor_interface = EditorInterface::get_singleton();

    // Configure connection lines
    set_connection_lines_curvature(0.0); // Straight lines
    set_connection_lines_thickness(2.0);
    set_connection_lines_antialiased(true);

    // Add new button
    new_button = memnew(Button);
    new_button->set_text("New Button");
    new_button->set_offset(godot::Side::SIDE_TOP, 50);
    new_button->set_offset(godot::Side::SIDE_LEFT, 10);
    new_button->connect("pressed", callable_mp(this, &GOAPGraphEditor::_on_new_button_pressed));
    new_button->set_z_index(1000); // High z-index to ensure it stays on top
    add_child(new_button);

    // Add context menu
    context_menu = memnew(PopupMenu);
    add_child(context_menu);
    context_menu->add_item("Add Action", 0);
    context_menu->add_item("Add Goal", 1);
    context_menu->connect("id_pressed", callable_mp(this, &GOAPGraphEditor::_on_context_menu_id_pressed));

    connect("node_selected", callable_mp(this, &GOAPGraphEditor::_on_node_selected));
    connect("node_deselected", callable_mp(this, &GOAPGraphEditor::_on_node_deselected));
    connect("delete_nodes_request", callable_mp(this, &GOAPGraphEditor::_on_delete_nodes_request));

    // Create debug instance selector
    debug_selector_container = memnew(HBoxContainer);
    debug_selector_container->set_offset(SIDE_LEFT, 12);
    debug_selector_container->set_offset(SIDE_TOP, 80);
    debug_selector_container->set_z_index(1000);
    add_child(debug_selector_container);

    debug_label = memnew(Label);
    debug_label->set_text("Debug Instance:");
    debug_selector_container->add_child(debug_label);

    debug_selector = memnew(OptionButton);
    debug_selector->add_item("None", 0);
    debug_selector->add_item("AAA", 1);
    debug_selector->add_item("BBB", 2);
    debug_selector->connect("item_selected", callable_mp(this, &GOAPGraphEditor::_on_debug_instance_selected));
    debug_selector_container->add_child(debug_selector);
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
    ClassDB::bind_method(D_METHOD("update_debug_view", "instance"), &GOAPGraphEditor::update_debug_view);
    ClassDB::bind_method(D_METHOD("_on_debug_instance_selected", "id"), &GOAPGraphEditor::_on_debug_instance_selected);
}

void GOAPGraphEditor::_on_node_selected(Node *node)
{
    if (node == nullptr)
        return;

    UtilityFunctions::print("Node selected: ", node->get_name());

    if (editor_interface && Engine::get_singleton()->is_editor_hint())
    {
        if (node->is_class("GOAPGraphNode"))
        {
            GOAPGraphNode *graph_node = Object::cast_to<GOAPGraphNode>(node);
            Object *asset = graph_node->get_asset_object();
            if (asset != nullptr)
            {
                editor_interface->inspect_object(asset);
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

namespace
{
void get_selected_nodes(const GraphEdit &graph_edit, std::vector<GOAPGraphNode *> &out_nodes)
{
    for (int i = 0; i < graph_edit.get_child_count(); ++i)
    {
        GOAPGraphNode *node_view = Object::cast_to<GOAPGraphNode>(graph_edit.get_child(i));
        if (node_view != nullptr)
        {
            if (node_view->is_selected())
            {
                out_nodes.push_back(node_view);
            }
        }
    }
}
} // namespace

void GOAPGraphEditor::_on_delete_nodes_request(Array node_names)
{
    std::vector<GOAPGraphNode *> to_erase;
    get_selected_nodes(*this, to_erase);

    for (auto &graph_node : to_erase)
    {
        UtilityFunctions::print("Node delete request: ", graph_node->get_name());

        Object *asset = graph_node->get_asset_object();
        if (asset != nullptr)
        {
            if (graph_node->node_type == GOAPGraphNode::NodeType::GOAL)
                goap_asset->remove_goal_asset(Ref<GOAPGoalAsset>(Object::cast_to<GOAPGoalAsset>(asset)));
            else if (graph_node->node_type == GOAPGraphNode::NodeType::ACTION)
                goap_asset->remove_action_asset(Ref<GOAPActionAsset>(Object::cast_to<GOAPActionAsset>(asset)));
        }

        remove_child(graph_node);
        if (godot::Engine::get_singleton()->is_editor_hint())
        {
            memdelete(graph_node);
        }
    }

    if (editor_interface && Engine::get_singleton()->is_editor_hint())
    {
        editor_interface->inspect_object(goap_asset.ptr());
    }
}

void GOAPGraphEditor::set_goap_asset(const Ref<GOAPAsset> &p_asset)
{
    if (goap_asset == p_asset)
        return;

    // Cleanup previous asset
    if (goap_asset != nullptr)
    {
        // Clear nodes
        for (int i = get_child_count() - 1; i >= 0; i--)
        {
            Node *child = get_child(i);
            if (child->is_class("GOAPGraphNode"))
            {
                remove_child(child);
                memdelete(child);
            }
        }

        goap_asset->disconnect("debug_instance_list_changed",
                               callable_mp(this, &GOAPGraphEditor::_on_debug_instance_list_changed));
    }

    print_line("GOAPGraphEditor::set_goap_asset " + p_asset->get_path());

    goap_asset = p_asset;

    // Events
    goap_asset->connect("debug_instance_list_changed",
                        callable_mp(this, &GOAPGraphEditor::_on_debug_instance_list_changed));

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
    if (p_action == nullptr)
        return;

    Label *label = memnew(Label);
    label->set_text(p_action->get_name());
    label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
    label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
    label->add_theme_font_size_override("font_size", 30);

    GOAPGraphNode *node = memnew(GOAPGraphNode);
    node->set_title("Action");
    node->set_position_offset(p_action->get_editor_position());
    node->set_name(p_action->get_name());
    node->set_action_asset(p_action);
    node->add_child(label);
    node->set_slot(0, true, 0, Color(1, 1, 1), true, 0, Color(1, 1, 1));

    if (p_action.is_valid())
    {
        p_action->connect("name_changed", callable_mp(node, &GOAPGraphNode::_on_asset_name_changed));
    }
    add_child(node);
}

void GOAPGraphEditor::add_goal_node(Ref<GOAPGoalAsset> p_goal)
{
    if (p_goal == nullptr)
        return;

    Label *label = memnew(Label);
    label->set_text(p_goal->get_name());
    label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
    label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
    label->add_theme_font_size_override("font_size", 30);

    GOAPGraphNode *node = memnew(GOAPGraphNode);
    node->set_title("Goal");
    node->set_position_offset(p_goal->get_editor_position());
    node->set_name(p_goal->get_name());
    node->set_goal_asset(p_goal);
    node->add_child(label);
    node->set_slot(0, true, 0, Color(1, 1, 1), false, 0, Color(1, 1, 1));

    if (p_goal.is_valid())
    {
        p_goal->connect("name_changed", callable_mp(node, &GOAPGraphNode::_on_asset_name_changed));
    }
    add_child(node);
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

void GOAPGraphEditor::_gui_input(const Ref<InputEvent> &p_event)
{
    Ref<InputEventMouseButton> mb = p_event;
    if (mb.is_valid() && mb->is_pressed())
    {
        if (mb->get_button_index() == MOUSE_BUTTON_RIGHT)
        {
            Vector2 local_pos = get_local_mouse_position();
            next_node_position = local_pos;

            context_menu->set_position(get_screen_position() + mb->get_position());
            context_menu->popup();
            accept_event();
        }
    }
}

PackedVector2Array GOAPGraphEditor::_get_connection_line(const Vector2 &p_from_position,
                                                         const Vector2 &p_to_position) const
{
    PackedVector2Array points;
    points.push_back(p_from_position);
    points.push_back(p_to_position);

    // Add directional triangle in the middle
    Vector2 dir = (p_to_position - p_from_position).normalized();
    Vector2 mid = (p_from_position + p_to_position) * 0.5f;
    Vector2 perp = Vector2(-dir.y, dir.x);

    float triangle_size = 10.0f;
    points.push_back(mid + dir * triangle_size);
    points.push_back(mid + perp * triangle_size * 0.5f);
    points.push_back(mid - perp * triangle_size * 0.5f);
    points.push_back(mid + dir * triangle_size);

    return points;
}

void GOAPGraphEditor::_on_new_button_pressed()
{
    if (goap_asset == nullptr)
        return;

    UtilityFunctions::print("New button pressed!");

    clear_connections();

    for (int i = 0; i < goap_asset->get_action_assets().size() - 1; i++)
    {
        GOAPActionAsset *from = goap_asset->get_action_asset(i).ptr();
        GOAPActionAsset *to = goap_asset->get_action_asset(i + 1).ptr();

        UtilityFunctions::print("Connecting ", from->get_name(), " to ", to->get_name());
        connect_node(from->get_name(), 0, to->get_name(), 0);
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

void GOAPGraphEditor::_on_debug_instance_selected(int p_id)
{
    UtilityFunctions::print("Debug instance selected: ", p_id);
    // TODO: Implement debug instance selection logic
}

void GOAPGraphEditor::_on_debug_instance_list_changed()
{
    // TODO: Implement debug instance changed logic
}