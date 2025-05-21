#include "GOAPPlanner.h"
#include "GOAPActionPlan.h"
#include <godot_cpp/core/class_db.hpp>

GOAPPlanner::GOAPPlanner() : debug_name(""), world_state(nullptr) {}

GOAPPlanner::~GOAPPlanner()
{
    for (auto &action : actions)
    {
        if (action.behaviour)
            memdelete(action.behaviour);
        if (action.condition)
            memdelete(action.condition);
        if (action.effect)
            memdelete(action.effect);
    }
    actions.clear();

    for (auto &goal : goals)
    {
        if (goal.condition)
            memdelete(goal.condition);
        if (goal.effect)
            memdelete(goal.effect);
    }
    goals.clear();

    if (world_state)
        memdelete(world_state);
}

void GOAPPlanner::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_debug_name"), &GOAPPlanner::get_debug_name);
    ClassDB::bind_method(D_METHOD("set_debug_name", "name"), &GOAPPlanner::set_debug_name);
    ClassDB::add_property("GOAPPlanner", PropertyInfo(Variant::STRING, "debug_name"), "SetDebugName", "GetDebugName");

    ClassDB::bind_method(D_METHOD("get_world_state"), &GOAPPlanner::get_world_state);

    ClassDB::bind_method(D_METHOD("is_action_valid", "actionIndex"), &GOAPPlanner::is_action_valid);
    ClassDB::bind_method(D_METHOD("get_action_count"), &GOAPPlanner::get_action_count);
    ClassDB::bind_method(D_METHOD("add_action", "action"), &GOAPPlanner::add_action);

    ClassDB::bind_method(D_METHOD("is_goal_valid", "goal_index"), &GOAPPlanner::is_goal_valid);
    ClassDB::bind_method(D_METHOD("get_goal_count"), &GOAPPlanner::get_goal_count);
    ClassDB::bind_method(D_METHOD("add_goal", "goal"), &GOAPPlanner::add_goal);
}

void GOAPPlanner::set_world_state(const WorldStateAsset *world_state_asset) noexcept
{
    if (world_state_asset == nullptr)
        return;

    world_state = world_state_asset->create_runtime_world_state();
}

void GOAPPlanner::add_action(const GOAPActionAsset *action) noexcept
{
    if (action == nullptr)
        return;

    GOAPPlanner::Action new_action = {};
    new_action.name = action->get_name();

    if (action->get_condition().is_valid())
    {
        Object *obj = action->get_condition().ptr()->new_();
        new_action.condition = dynamic_cast<GOAPCondition *>(obj);
    }

    if (action->get_effect().is_valid())
    {
        Object *obj = action->get_effect().ptr()->new_();
        new_action.effect = dynamic_cast<GOAPEffect *>(obj);
    }

    if (action->get_action().is_valid())
    {
        Object *obj = action->get_action().ptr()->new_();
        new_action.behaviour = dynamic_cast<GOAPActionBehaviour *>(obj);
    }

    actions.push_back(new_action);
}

void GOAPPlanner::add_goal(const GOAPGoalAsset *goal) noexcept
{
    if (goal == nullptr)
        return;

    GOAPPlanner::Goal new_goal = {};
    new_goal.name = goal->get_name();
    new_goal.priority = goal->get_priority();

    if (goal->get_condition().is_valid())
    {
        Object *obj = goal->get_condition().ptr()->new_();
        new_goal.condition = dynamic_cast<GOAPCondition *>(obj);
    }

    if (goal->get_effect().is_valid())
    {
        Object *obj = goal->get_effect().ptr()->new_();
        new_goal.effect = dynamic_cast<GOAPEffect *>(obj);
    }

    goals.push_back(new_goal);
}

bool GOAPPlanner::check_if_plan_success(const Goal &InGoal, const WorldState *InWorldState,
                                        class GOAPActionPlan *InTestPlan, WorldState *TempWs)
{
    bool bSuccess = true;

    TempWs->copy_data_from(InWorldState);

    for (int Index = InTestPlan->Length() - 1; Index >= 0; --Index)
    {
        const auto *Action = InTestPlan->Get(Index);
        if (!Action->is_pass_condition(TempWs))
        {
            bSuccess = false;
            break;
        }
        Action->apply_effect(TempWs);
    }

    bSuccess = bSuccess && InGoal.is_achieved(InWorldState);
    return bSuccess;
}

bool GOAPPlanner::search_plan_brute_force(class GOAPActionPlan *OutPlan, const WorldState *InWorldState,
                                          const Goal &InGoal, const std::vector<Action> &InActions)
{
    bool bSuccess = false;

    int NumActions = InActions.size();

    WorldState TempWs = {};
    TempWs.snapshot_from(InWorldState);

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

        if (permute_test_plan(OutPlan, InGoal, InWorldState, 0, &TempWs))
        {
            bSuccess = true;
            break;
        }
    }

    return bSuccess;
}

bool GOAPPlanner::permute_test_plan(class GOAPActionPlan *InOutPlan, const Goal &InGoal, const WorldState *InWorldState,
                                    int InCurrentIndex, WorldState *TempWs)
{
    if (InCurrentIndex == InOutPlan->Length())
    {
        return check_if_plan_success(InGoal, InWorldState, InOutPlan, TempWs);
    }

    for (int i = InCurrentIndex; i < InOutPlan->Length(); i++)
    {
        InOutPlan->Swap(InCurrentIndex, i);

        if (permute_test_plan(InOutPlan, InGoal, InWorldState, InCurrentIndex + 1, TempWs))
        {
            return true;
        }

        InOutPlan->Swap(InCurrentIndex, i);
    }

    return false;
}