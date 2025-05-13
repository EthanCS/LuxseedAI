#ifndef GOAP_PLANNER_H
#define GOAP_PLANNER_H

#include "GOAPActionBehaviour.h"
#include "GOAPCondition.h"
#include "GOAPEffect.h"
#include "WorldState.h"
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <vector>

using namespace godot;

class GOAPPlanner : public Object
{
    GDCLASS(GOAPPlanner, Object)

public:
    struct Goal
    {
        String Name;
        int Priority;
        GOAPCondition *Condition;
        GOAPEffect *Effect;

        bool IsAchieved(const WorldState *ws) const
        {
            if (Condition)
                return Condition->IsPass(ws);
            return false;
        }

        void ApplyEffect(WorldState *ws) const
        {
            if (Effect)
                Effect->TakeEffect(ws);
        }
    };

    struct Action
    {
        String Name;
        GOAPActionBehaviour *Behaviour;
        GOAPCondition *Condition;
        GOAPEffect *Effect;

        bool IsPassCondition(const WorldState *ws) const
        {
            if (Condition)
                return Condition->IsPass(ws);
            return true;
        }

        void ApplyEffect(WorldState *ws) const
        {
            if (Effect)
                Effect->TakeEffect(ws);
        }

        void SyncWorldStateProperty(const WorldState *ws) {}

        void Start() { Behaviour->Start(); }
        void Update(float deltaTime) { Behaviour->Update(deltaTime); }
        bool IsDone() const { return Behaviour->IsDone(); }
        void End() { Behaviour->End(); }
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

    static bool SearchPlan_BruteForce(class GOAPActionPlan *OutPlan, const WorldState *Ws, const Goal &InGoal,
                                      const std::vector<Action> &InActions);
    static bool PermuteTestPlan(class GOAPActionPlan *OutPlan, const Goal &InGoal, const WorldState *Ws,
                                int CurrentIndex, WorldState *TestSnapshot);

public:
    GOAPPlanner();

    String GetDebugName() const noexcept { return debugName; }
    void SetDebugName(const String &name) noexcept { debugName = name; }

    WorldState *GetWorldState() const noexcept { return worldState; }
    void SetWorldState(WorldState *ws) noexcept { worldState = ws; }

    bool IsActionValid(int index) const noexcept { return index >= 0 && index < actions.size(); }
    int GetNumActions() const noexcept { return actions.size(); }
    void AddAction(const String &name, GOAPActionBehaviour *action, GOAPCondition *condition,
                   GOAPEffect *effect) noexcept
    {
        actions.push_back({name, action, condition, effect});
    }

    bool IsGoalValid(int index) const noexcept { return index >= 0 && index < goals.size(); }
    int GetNumGoals() const noexcept { return goals.size(); }
    void AddGoal(const String &name, int priority, GOAPCondition *condition, GOAPEffect *effect) noexcept
    {
        goals.push_back({name, priority, condition, effect});
    }

public:
    Action *GetAction(int index) noexcept { return &actions[index]; }
    const Action *GetAction(int index) const noexcept { return &actions[index]; }

    Goal *GetGoal(int index) noexcept { return &goals[index]; }
    const Goal *GetGoal(int index) const noexcept { return &goals[index]; }
};

#endif