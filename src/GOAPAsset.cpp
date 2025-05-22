#include "GOAPAsset.h"
#include "GOAPPlanner.h"
#include "WorldStateAsset.h"

//////////////////////////////////////////////////////////////////////////////////////
// GOAPGoalAsset
//////////////////////////////////////////////////////////////////////////////////////
GOAPGoalAsset::GOAPGoalAsset() : name(""), priority(0) {}

void GOAPGoalAsset::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_name"), &GOAPGoalAsset::get_name);
    ClassDB::bind_method(D_METHOD("set_name", "name"), &GOAPGoalAsset::set_name);

    ClassDB::bind_method(D_METHOD("get_priority"), &GOAPGoalAsset::get_priority);
    ClassDB::bind_method(D_METHOD("set_priority", "priority"), &GOAPGoalAsset::set_priority);

    ClassDB::bind_method(D_METHOD("get_condition"), &GOAPGoalAsset::get_condition);
    ClassDB::bind_method(D_METHOD("set_condition", "condition"), &GOAPGoalAsset::set_condition);

    ClassDB::bind_method(D_METHOD("get_effect"), &GOAPGoalAsset::get_effect);
    ClassDB::bind_method(D_METHOD("set_effect", "effect"), &GOAPGoalAsset::set_effect);

    ClassDB::bind_method(D_METHOD("get_editor_position"), &GOAPGoalAsset::get_editor_position);
    ClassDB::bind_method(D_METHOD("set_editor_position", "position"), &GOAPGoalAsset::set_editor_position);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "priority"), "set_priority", "get_priority");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "condition", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_condition",
                 "get_condition");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_effect",
                 "get_effect");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "editor_position"), "set_editor_position", "get_editor_position");
}

const String &GOAPGoalAsset::get_name() const { return name; }
void GOAPGoalAsset::set_name(const String &p_name) { name = p_name; }

int GOAPGoalAsset::get_priority() const { return priority; }
void GOAPGoalAsset::set_priority(int p_priority) { priority = p_priority; }

//////////////////////////////////////////////////////////////////////////////////////
// GOAPActionAsset
//////////////////////////////////////////////////////////////////////////////////////
GOAPActionAsset::GOAPActionAsset() : name("") {}

void GOAPActionAsset::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_name"), &GOAPActionAsset::get_name);
    ClassDB::bind_method(D_METHOD("set_name", "name"), &GOAPActionAsset::set_name);

    ClassDB::bind_method(D_METHOD("get_condition"), &GOAPActionAsset::get_condition);
    ClassDB::bind_method(D_METHOD("set_condition", "condition"), &GOAPActionAsset::set_condition);

    ClassDB::bind_method(D_METHOD("get_effect"), &GOAPActionAsset::get_effect);
    ClassDB::bind_method(D_METHOD("set_effect", "effect"), &GOAPActionAsset::set_effect);

    ClassDB::bind_method(D_METHOD("get_action"), &GOAPActionAsset::get_action);
    ClassDB::bind_method(D_METHOD("set_action", "action"), &GOAPActionAsset::set_action);

    ClassDB::bind_method(D_METHOD("get_editor_position"), &GOAPActionAsset::get_editor_position);
    ClassDB::bind_method(D_METHOD("set_editor_position", "position"), &GOAPActionAsset::set_editor_position);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "condition", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_condition",
                 "get_condition");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_effect",
                 "get_effect");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "action", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_action",
                 "get_action");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "editor_position"), "set_editor_position", "get_editor_position");
}

const String &GOAPActionAsset::get_name() const { return name; }
void GOAPActionAsset::set_name(const String &p_name) { name = p_name; }

//////////////////////////////////////////////////////////////////////////////////////
// GOAPAsset
//////////////////////////////////////////////////////////////////////////////////////
GOAPAsset::GOAPAsset()
{
    goal_assets.clear();
    action_assets.clear();
}

void GOAPAsset::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("create_planner"), &GOAPAsset::create_planner);

    ClassDB::bind_method(D_METHOD("get_world_state_asset"), &GOAPAsset::get_world_state_asset);
    ClassDB::bind_method(D_METHOD("set_world_state_asset", "ws"), &GOAPAsset::set_world_state_asset);

    ClassDB::bind_method(D_METHOD("get_goal_assets"), &GOAPAsset::get_goal_assets);
    ClassDB::bind_method(D_METHOD("set_goal_assets", "goals"), &GOAPAsset::set_goal_assets);

    ClassDB::bind_method(D_METHOD("get_action_assets"), &GOAPAsset::get_action_assets);
    ClassDB::bind_method(D_METHOD("set_action_assets", "actions"), &GOAPAsset::set_action_assets);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "world_state_asset", PROPERTY_HINT_RESOURCE_TYPE, "WorldStateAsset"),
                 "set_world_state_asset", "get_world_state_asset");
    ADD_PROPERTY(
        PropertyInfo(Variant::ARRAY, "goals", PROPERTY_HINT_TYPE_STRING,
                     String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":GOAPGoalAsset"),
        "set_goal_assets", "get_goal_assets");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "actions", PROPERTY_HINT_TYPE_STRING,
                              String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) +
                                  ":GOAPActionAsset"),
                 "set_action_assets", "get_action_assets");
}

void GOAPAsset::set_goal_assets(const TypedArray<GOAPGoalAsset> &p_goals) { goal_assets = p_goals; }
TypedArray<GOAPGoalAsset> GOAPAsset::get_goal_assets() const { return goal_assets; }

void GOAPAsset::set_action_assets(const TypedArray<GOAPActionAsset> &p_actions) { action_assets = p_actions; }
TypedArray<GOAPActionAsset> GOAPAsset::get_action_assets() const { return action_assets; }

GOAPPlanner *GOAPAsset::create_planner() const
{
    if (!world_state_asset.is_valid())
    {
        print_error("GOAPAsset::create_planner: worldState is null");
        return nullptr;
    }

    GOAPPlanner *planner = memnew(GOAPPlanner);
    planner->set_world_state_from_asset(world_state_asset.ptr());

    for (int i = 0; i < goal_assets.size(); i++)
    {
        if (goal_assets[i].get_type() == Variant::NIL)
            continue;

        Ref<GOAPGoalAsset> goal = goal_assets[i];
        planner->add_goal(goal.ptr());
    }

    for (int i = 0; i < action_assets.size(); i++)
    {
        if (action_assets[i].get_type() == Variant::NIL)
            continue;

        Ref<GOAPActionAsset> action = action_assets[i];
        planner->add_action(action.ptr());
    }

    return planner;
}

Ref<GOAPActionAsset> GOAPAsset::new_action_asset(const String &p_name, Vector2 p_editor_position)
{
    Ref<GOAPActionAsset> action = memnew(GOAPActionAsset);
    action->set_name(p_name);
    action->set_editor_position(p_editor_position);
    action_assets.push_back(action);
    return action;
}

Ref<GOAPGoalAsset> GOAPAsset::new_goal_asset(const String &p_name, Vector2 p_editor_position)
{
    Ref<GOAPGoalAsset> goal = memnew(GOAPGoalAsset);
    goal->set_name(p_name);
    goal->set_editor_position(p_editor_position);
    goal_assets.push_back(goal);
    return goal;
}