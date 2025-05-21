#include "GOAPActionPlan.h"
#include "GOAPPlanner.h"
#include "godot_cpp/core/print_string.hpp"
#include <godot_cpp/core/class_db.hpp>

GOAPActionPlan::GOAPActionPlan() {}

void GOAPActionPlan::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("IsFinished"), &GOAPActionPlan::IsFinished);
    ClassDB::bind_method(D_METHOD("IsEmpty"), &GOAPActionPlan::IsEmpty);
    ClassDB::bind_method(D_METHOD("Length"), &GOAPActionPlan::Length);

    ClassDB::bind_method(D_METHOD("Reset"), &GOAPActionPlan::Reset);
    ClassDB::bind_method(D_METHOD("Execute", "InWorldState", "InDeltaTime"), &GOAPActionPlan::Execute);
}

void GOAPActionPlan::Reset()
{
    goalIndex = -1;
    actionSequences.clear();
    curActionIndex = -1;
    bNeedStartAction = true;
    bFinished = false;
}

void GOAPActionPlan::Execute(WorldState *InWorldState, float InDeltaTime)
{
    if (InWorldState == nullptr || planner == nullptr)
    {
        godot::print_error("GOAPActionPlan::Execute: InWorldState or planner is nullptr");
        return;
    }

    if (!planner->is_goal_valid(goalIndex))
    {
        godot::print_error("GOAPActionPlan::Execute: Invalid goal index {}", goalIndex);
        return;
    }

    if (IsFinished() || IsEmpty())
        return;

    if (!planner->is_action_valid(curActionIndex))
        return;

    GOAPPlanner::Action *action = planner->get_action(curActionIndex);
    action->SyncWorldStateProperty(InWorldState);

    if (bNeedStartAction)
    {
        action->start();
        bNeedStartAction = false;
    }

    action->update(InDeltaTime);

    if (action->is_done())
    {
        action->end();
        action->apply_effect(InWorldState);
        curActionIndex++;
        bNeedStartAction = true;
    }

    if (curActionIndex >= actionSequences.size())
    {
        curActionIndex = actionSequences.size() - 1;
        bFinished = true;
        GOAPPlanner::Goal *goal = planner->get_goal(goalIndex);
        goal->apply_effect(InWorldState);
    }
}