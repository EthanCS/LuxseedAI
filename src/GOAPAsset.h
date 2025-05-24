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

protected:
    void _name_changed();
    int priority;
    Ref<GDScript> condition;
    Ref<GDScript> effect;

    Vector2 editor_position;

protected:
    static void _bind_methods();

public:
    GOAPGoalAsset();

    void set_name(const String &p_name);
    const String &get_name() const noexcept { return name; }

    void set_priority(int p_priority);
    int get_priority() const;

    void set_condition(const Ref<GDScript> &p_condition) noexcept { condition = p_condition; }
    Ref<GDScript> get_condition() const noexcept { return condition; }

    void set_effect(const Ref<GDScript> &p_effect) noexcept { effect = p_effect; }
    Ref<GDScript> get_effect() const noexcept { return effect; }

    void set_editor_position(const Vector2 &p_position) noexcept { editor_position = p_position; }
    Vector2 get_editor_position() const noexcept { return editor_position; }
};

//////////////////////////////////////////////////////////////////////////////////////
// GOAPActionAsset
//////////////////////////////////////////////////////////////////////////////////////
class GOAPActionAsset : public Resource
{
    GDCLASS(GOAPActionAsset, Resource)

private:
    String name;

protected:
    void _name_changed();
    Ref<GDScript> condition;
    Ref<GDScript> effect;
    Ref<GDScript> action;

    Vector2 editor_position;

protected:
    static void _bind_methods();

public:
    GOAPActionAsset();

    void set_name(const String &p_name);
    const String &get_name() const noexcept { return name; }

    void set_condition(const Ref<GDScript> &p_condition) noexcept { condition = p_condition; }
    Ref<GDScript> get_condition() const noexcept { return condition; }

    void set_effect(const Ref<GDScript> &p_effect) noexcept { effect = p_effect; }
    Ref<GDScript> get_effect() const noexcept { return effect; }

    void set_action(const Ref<GDScript> &p_action) noexcept { action = p_action; }
    Ref<GDScript> get_action() const noexcept { return action; }

    void set_editor_position(const Vector2 &p_position) noexcept { editor_position = p_position; }
    Vector2 get_editor_position() const noexcept { return editor_position; }
};

//////////////////////////////////////////////////////////////////////////////////////
// GOAPAsset
//////////////////////////////////////////////////////////////////////////////////////
class GOAPAsset : public Resource
{
    GDCLASS(GOAPAsset, Resource)

private:
    Ref<WorldStateAsset> world_state_asset;
    TypedArray<GOAPGoalAsset> goal_assets;
    TypedArray<GOAPActionAsset> action_assets;

protected:
    static void _bind_methods();

public:
    GOAPAsset();

    void set_goal_assets(const TypedArray<GOAPGoalAsset> &p_goals);
    TypedArray<GOAPGoalAsset> get_goal_assets() const;
    Ref<GOAPGoalAsset> get_goal_asset(int p_index) const { return goal_assets[p_index]; }
    void remove_goal_asset(Ref<GOAPGoalAsset> p_goal) { goal_assets.erase(p_goal); }

    void set_action_assets(const TypedArray<GOAPActionAsset> &p_actions);
    TypedArray<GOAPActionAsset> get_action_assets() const;
    Ref<GOAPActionAsset> GOAPAsset::get_action_asset(int p_index) const { return action_assets[p_index]; }
    void remove_action_asset(Ref<GOAPActionAsset> p_action) { action_assets.erase(p_action); }

    void set_world_state_asset(const Ref<WorldStateAsset> &p_world_state) { world_state_asset = p_world_state; }
    Ref<WorldStateAsset> get_world_state_asset() const { return world_state_asset; }

    class GOAPPlanner *create_planner() const;

    Ref<GOAPActionAsset> new_action_asset(const String &p_name, Vector2 p_editor_position = Vector2(0, 0));
    Ref<GOAPGoalAsset> new_goal_asset(const String &p_name, Vector2 p_editor_position = Vector2(0, 0));

    // Debug
private:
    std::vector<class GOAPPlanner *> debug_instances;

public:
    void set_debug(class GOAPPlanner *p_planner, bool p_debugging);
};

#endif // GOAP_ASSET_H