#include "GOAPPlanner.h"
#include "GOAPActionPlan.h"
#include <godot_cpp/core/class_db.hpp>

GOAPPlanner::GOAPPlanner() {}

void GOAPPlanner::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("GetDebugName"), &GOAPPlanner::GetDebugName);
    ClassDB::bind_method(D_METHOD("SetDebugName", "name"), &GOAPPlanner::SetDebugName);
    ClassDB::add_property("GOAPPlanner", PropertyInfo(Variant::STRING, "debugName"), "SetDebugName", "GetDebugName");

    ClassDB::bind_method(D_METHOD("GetWorldState"), &GOAPPlanner::GetWorldState);
    ClassDB::bind_method(D_METHOD("SetWorldState", "worldState"), &GOAPPlanner::SetWorldState);

    ClassDB::bind_method(D_METHOD("IsActionValid", "actionIndex"), &GOAPPlanner::IsActionValid);
    ClassDB::bind_method(D_METHOD("GetNumActions"), &GOAPPlanner::GetNumActions);
    ClassDB::bind_method(D_METHOD("AddAction", "name", "behaviour", "condition", "effect"), &GOAPPlanner::AddAction);

    ClassDB::bind_method(D_METHOD("IsGoalValid", "goalIndex"), &GOAPPlanner::IsGoalValid);
    ClassDB::bind_method(D_METHOD("GetNumGoals"), &GOAPPlanner::GetNumGoals);
    ClassDB::bind_method(D_METHOD("AddGoal", "name", "priority", "condition", "effect"), &GOAPPlanner::AddGoal);
}

bool GOAPPlanner::CheckIfPlanSuccess(const Goal &InGoal, const WorldState *InWorldState,
                                     class GOAPActionPlan *InTestPlan, WorldState *TempWs)
{
    bool bSuccess = true;

    TempWs->CopyDataFrom(InWorldState);

    for (int Index = InTestPlan->Length() - 1; Index >= 0; --Index)
    {
        const auto *Action = InTestPlan->Get(Index);
        if (!Action->IsPassCondition(TempWs))
        {
            bSuccess = false;
            break;
        }
        Action->ApplyEffect(TempWs);
    }

    bSuccess = bSuccess && InGoal.IsAchieved(InWorldState);
    return bSuccess;
}

bool GOAPPlanner::SearchPlan_BruteForce(class GOAPActionPlan *OutPlan, const WorldState *InWorldState,
                                        const Goal &InGoal, const std::vector<Action> &InActions)
{
    bool bSuccess = false;

    int NumActions = InActions.size();

    WorldState TempWs = {};
    TempWs.SnapshotFrom(InWorldState);

    for (int mask = 1; mask < (1 << NumActions); mask++)
    {
        OutPlan->Reset();

        for (int i = 0; i < NumActions; i++)
        {
            if ((mask & (1 << i)) != 0)
            {
                OutPlan->PushBack(i);
            }
        }

        if (PermuteTestPlan(OutPlan, InGoal, InWorldState, 0, &TempWs))
        {
            bSuccess = true;
            break;
        }
    }

    return bSuccess;
}

bool GOAPPlanner::PermuteTestPlan(class GOAPActionPlan *InOutPlan, const Goal &InGoal, const WorldState *InWorldState,
                                  int InCurrentIndex, WorldState *TempWs)
{
    if (InCurrentIndex == InOutPlan->Length())
    {
        return CheckIfPlanSuccess(InGoal, InWorldState, InOutPlan, TempWs);
    }

    for (int i = InCurrentIndex; i < InOutPlan->Length(); i++)
    {
        InOutPlan->Swap(InCurrentIndex, i);

        if (PermuteTestPlan(InOutPlan, InGoal, InWorldState, InCurrentIndex + 1, TempWs))
        {
            return true;
        }

        InOutPlan->Swap(InCurrentIndex, i);
    }

    return false;
}