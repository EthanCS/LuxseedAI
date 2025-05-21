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

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "priority"), "set_priority", "get_priority");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "condition", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_condition",
                 "get_condition");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_effect",
                 "get_effect");
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

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "condition", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_condition",
                 "get_condition");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_effect",
                 "get_effect");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "action", PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_action",
                 "get_action");
}

const String &GOAPActionAsset::get_name() const { return name; }
void GOAPActionAsset::set_name(const String &p_name) { name = p_name; }

//////////////////////////////////////////////////////////////////////////////////////
// GOAPAsset
//////////////////////////////////////////////////////////////////////////////////////
GOAPAsset::GOAPAsset()
{
    goals.clear();
    actions.clear();
}

void GOAPAsset::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("create_planner"), &GOAPAsset::create_planner);

    ClassDB::bind_method(D_METHOD("get_world_state"), &GOAPAsset::get_world_state);
    ClassDB::bind_method(D_METHOD("set_world_state", "ws"), &GOAPAsset::set_world_state);

    ClassDB::bind_method(D_METHOD("get_goals"), &GOAPAsset::get_goals);
    ClassDB::bind_method(D_METHOD("set_goals", "goals"), &GOAPAsset::set_goals);

    ClassDB::bind_method(D_METHOD("get_actions"), &GOAPAsset::get_actions);
    ClassDB::bind_method(D_METHOD("set_actions", "actions"), &GOAPAsset::set_actions);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "world_state", PROPERTY_HINT_RESOURCE_TYPE, "WorldStateAsset"),
                 "set_world_state", "get_world_state");
    ADD_PROPERTY(
        PropertyInfo(Variant::ARRAY, "goals", PROPERTY_HINT_TYPE_STRING,
                     String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":GOAPGoalAsset"),
        "set_goals", "get_goals");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "actions", PROPERTY_HINT_TYPE_STRING,
                              String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) +
                                  ":GOAPActionAsset"),
                 "set_actions", "get_actions");
}

void GOAPAsset::set_goals(const TypedArray<GOAPGoalAsset> &p_goals) { goals = p_goals; }
TypedArray<GOAPGoalAsset> GOAPAsset::get_goals() const { return goals; }

void GOAPAsset::set_actions(const TypedArray<GOAPActionAsset> &p_actions) { actions = p_actions; }
TypedArray<GOAPActionAsset> GOAPAsset::get_actions() const { return actions; }

GOAPPlanner *GOAPAsset::create_planner() const
{
    GOAPPlanner *planner = memnew(GOAPPlanner);
    return planner;
}
