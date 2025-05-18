#ifndef GOAP_ASSET_H
#define GOAP_ASSET_H

#include "godot_cpp/core/binder_common.hpp"
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/script.hpp>

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
    Ref<Script> condition;
    Ref<Script> effect;

protected:
    static void _bind_methods();

public:
    GOAPGoalAsset();

    void set_name(const String &p_name);
    const String &get_name() const;

    void set_priority(int p_priority);
    int get_priority() const;

    void set_condition(const Ref<Script> &p_condition) { condition = p_condition; }
    Ref<Script> get_condition() const { return condition; }

    void set_effect(const Ref<Script> &p_effect) { effect = p_effect; }
    Ref<Script> get_effect() const { return effect; }
};

//////////////////////////////////////////////////////////////////////////////////////
// GOAPActionAsset
//////////////////////////////////////////////////////////////////////////////////////
class GOAPActionAsset : public Resource
{
    GDCLASS(GOAPActionAsset, Resource)

private:
    String name;
    Ref<Script> condition;
    Ref<Script> effect;
    Ref<Script> action;

protected:
    static void _bind_methods();

public:
    GOAPActionAsset();

    void set_name(const String &p_name);
    const String &get_name() const;

    void set_condition(const Ref<Script> &p_condition) { condition = p_condition; }
    Ref<Script> get_condition() const { return condition; }

    void set_effect(const Ref<Script> &p_effect) { effect = p_effect; }
    Ref<Script> get_effect() const { return effect; }

    void set_action(const Ref<Script> &p_action) { action = p_action; }
    Ref<Script> get_action() const { return action; }
};

//////////////////////////////////////////////////////////////////////////////////////
// GOAPAsset
//////////////////////////////////////////////////////////////////////////////////////
class GOAPAsset : public Resource
{
    GDCLASS(GOAPAsset, Resource)

private:
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
};

#endif // GOAP_ASSET_H