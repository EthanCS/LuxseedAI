#ifndef GOAP_ASSET_H
#define GOAP_ASSET_H

#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/resource.hpp>

#include "WorldStateAsset.h"

using namespace godot;

//////////////////////////////////////////////////////////////////////////////////////
// GOAPGoalAsset
//////////////////////////////////////////////////////////////////////////////////////
class GOAPGoalAsset : public Resource
{
    GDCLASS(GOAPGoalAsset, Resource)

private:
    String name;
    int priority;
    Ref<GDScript> condition;
    Ref<GDScript> effect;

protected:
    static void _bind_methods();

public:
    GOAPGoalAsset();

    void set_name(const String &p_name);
    const String &get_name() const;

    void set_priority(int p_priority);
    int get_priority() const;

    void set_condition(const Ref<GDScript> &p_condition) { condition = p_condition; }
    Ref<GDScript> get_condition() const { return condition; }

    void set_effect(const Ref<GDScript> &p_effect) { effect = p_effect; }
    Ref<GDScript> get_effect() const { return effect; }
};

//////////////////////////////////////////////////////////////////////////////////////
// GOAPActionAsset
//////////////////////////////////////////////////////////////////////////////////////
class GOAPActionAsset : public Resource
{
    GDCLASS(GOAPActionAsset, Resource)

private:
    String name;
    Ref<GDScript> condition;
    Ref<GDScript> effect;
    Ref<GDScript> action;

protected:
    static void _bind_methods();

public:
    GOAPActionAsset();

    void set_name(const String &p_name);
    const String &get_name() const;

    void set_condition(const Ref<GDScript> &p_condition) { condition = p_condition; }
    Ref<GDScript> get_condition() const { return condition; }

    void set_effect(const Ref<GDScript> &p_effect) { effect = p_effect; }
    Ref<GDScript> get_effect() const { return effect; }

    void set_action(const Ref<GDScript> &p_action) { action = p_action; }
    Ref<GDScript> get_action() const { return action; }
};

//////////////////////////////////////////////////////////////////////////////////////
// GOAPAsset
//////////////////////////////////////////////////////////////////////////////////////
class GOAPAsset : public Resource
{
    GDCLASS(GOAPAsset, Resource)

private:
    Ref<WorldStateAsset> worldState;
    TypedArray<GOAPGoalAsset> goals;
    TypedArray<GOAPActionAsset> actions;

protected:
    static void _bind_methods();

public:
    GOAPAsset();

    void set_goals(const TypedArray<GOAPGoalAsset> &p_goals);
    TypedArray<GOAPGoalAsset> get_goals() const;

    void set_actions(const TypedArray<GOAPActionAsset> &p_actions);
    TypedArray<GOAPActionAsset> get_actions() const;

    void set_world_state(const Ref<WorldStateAsset> &p_world_state) { worldState = p_world_state; }
    Ref<WorldStateAsset> get_world_state() const { return worldState; }

    class GOAPPlanner *create_planner() const;
};

#endif // GOAP_ASSET_H