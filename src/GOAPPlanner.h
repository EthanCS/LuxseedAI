#ifndef GOAP_PLANNER_H
#define GOAP_PLANNER_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <vector>

#include "GOAPActionBehaviour.h"
#include "GOAPAsset.h"
#include "GOAPCondition.h"
#include "GOAPEffect.h"
#include "WorldState.h"

using namespace godot;

class GOAPPlanner : public Object
{
    GDCLASS(GOAPPlanner, Object)

public:
    struct Goal
    {
        String name;
        int priority;
        GOAPCondition *condition;
        GOAPEffect *effect;

        bool is_achieved(const WorldState *ws) const
        {
            if (condition)
                return condition->is_pass(ws);
            return false;
        }

        void apply_effect(WorldState *ws) const
        {
            if (effect)
                effect->apply(ws);
        }
    };

    struct Action
    {
        String name;
        GOAPActionBehaviour *behaviour;
        GOAPCondition *condition;
        GOAPEffect *effect;

        bool is_pass_condition(const WorldState *ws) const
        {
            if (condition)
                return condition->is_pass(ws);
            return true;
        }

        void apply_effect(WorldState *ws) const
        {
            if (effect)
                effect->apply(ws);
        }

        void sync_world_state_variable(const WorldState *ws) {}

        void start() { behaviour->start(); }
        void update(float dt) { behaviour->update(dt); }
        bool is_done() const { return behaviour->is_done(); }
        void end() { behaviour->end(); }
    };

private:
    String debug_name;
    WorldState *world_state;
    std::vector<Goal> goals;
    std::vector<Action> actions;

protected:
    static void _bind_methods();

    static bool check_if_plan_success(const Goal &InGoal, const WorldState *Ws, class GOAPActionPlan *OutPlan,
                                      WorldState *TestSnapshot);

    static bool search_plan_brute_force(class GOAPActionPlan *OutPlan, const WorldState *Ws, const Goal &InGoal,
                                        const std::vector<Action> &InActions);
    static bool permute_test_plan(class GOAPActionPlan *OutPlan, const Goal &InGoal, const WorldState *Ws,
                                  int CurrentIndex, WorldState *TestSnapshot);

public:
    String get_debug_name() const noexcept { return debug_name; }
    void set_debug_name(const String &name) noexcept { debug_name = name; }

    WorldState *get_world_state() const noexcept { return world_state; }
    void set_world_state_from_asset(const WorldStateAsset *world_state_asset) noexcept;

    void add_action(const GOAPActionAsset *action) noexcept;
    bool is_action_valid(int index) const noexcept { return index >= 0 && index < actions.size(); }
    int get_action_count() const noexcept { return actions.size(); }

    void add_goal(const GOAPGoalAsset *goal) noexcept;
    bool is_goal_valid(int index) const noexcept { return index >= 0 && index < goals.size(); }
    int get_goal_count() const noexcept { return goals.size(); }

public:
    GOAPPlanner();
    virtual ~GOAPPlanner();

    Action *get_action(int index) noexcept
    {
        if (!is_action_valid(index))
            return nullptr;
        return &actions[index];
    }

    Goal *get_goal(int index) noexcept
    {
        if (!is_goal_valid(index))
            return nullptr;
        return &goals[index];
    }

public:
#ifdef TOOLS_ENABLED
    Ref<GOAPAsset> asset;
    
    void set_debug(bool p_debugging)
    {
        if (asset != nullptr)
            asset->set_debug(this, p_debugging);
    }
#endif
};

#endif