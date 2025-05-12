#ifndef GOAP_PLANNER_H
#define GOAP_PLANNER_H

#include "GOAPAction.h"
#include "WorldState.h"
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <vector>

using namespace godot;

class GOAPPlanner : public Object
{
    GDCLASS(GOAPPlanner, Object)

    struct Goal
    {
        String Name;
        int Priority;
    };

    struct Action
    {
        String Name;
        GOAPAction *Behaviour;
    };

private:
    String debugName;
    WorldState *worldState;
    std::vector<Goal> goals;
    std::vector<Action> actions;

protected:
    static void _bind_methods();

    static bool CheckIfPlanSuccess(const Goal &InGoal, const WorldState *Ws, class GOAPActionPlan *OutPlan,
                                   WorldState *TestSnapshot);

public:
    GOAPPlanner();

    String GetDebugName() const noexcept { return debugName; }
    void SetDebugName(const String &name) noexcept { debugName = name; }

    WorldState *GetWorldState() const noexcept { return worldState; }
    void SetWorldState(WorldState *ws) noexcept { worldState = ws; }

    int GetNumActions() const noexcept { return actions.size(); }
    void AddAction(const String &name, GOAPAction *action) noexcept { actions.push_back({name, action}); }

    int GetNumGoals() const noexcept { return goals.size(); }
    void AddGoal(const String &name, int priority) noexcept { goals.push_back({name, priority}); }
};

#endif