#ifndef GOAP_ACTION_PLAN_H
#define GOAP_ACTION_PLAN_H

#include "GOAPActionBehaviour.h"
#include "GOAPPlanner.h"
#include "WorldState.h"
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <vector>

using namespace godot;

class GOAPActionPlan : public Object
{
    GDCLASS(GOAPActionPlan, Object)

private:
    GOAPPlanner *planner;
    int goalIndex;
    std::vector<int> actionSequences;
    int curActionIndex;
    bool bNeedStartAction;
    bool bFinished;

protected:
    static void _bind_methods();

public:
    GOAPActionPlan();

    bool IsFinished() const noexcept { return bFinished; }
    bool IsEmpty() const noexcept { return actionSequences.empty(); }
    int Length() const noexcept { return actionSequences.size(); }

    void Reset();
    void Execute(WorldState *InWorldState, float InDeltaTime);

public:
    void Swap(int i, int j)
    {
        int temp = actionSequences[i];
        actionSequences[i] = actionSequences[j];
        actionSequences[j] = temp;
    }

    GOAPPlanner *GetPlanner() const noexcept { return planner; }

    GOAPPlanner::Action *Get(int Index) noexcept { return planner->get_action(actionSequences[Index]); }
    const GOAPPlanner::Action *Get(int Index) const noexcept { return planner->get_action(actionSequences[Index]); }

    void PushBack(int ActionIndex) { actionSequences.push_back(ActionIndex); }
    void Reverse() { std::reverse(actionSequences.begin(), actionSequences.end()); }
};

#endif