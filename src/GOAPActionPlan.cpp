#include "GOAPActionPlan.h"
#include "GOAPPlanner.h"
#include "godot_cpp/core/print_string.hpp"
#include <godot_cpp/core/class_db.hpp>

GOAPActionPlan::GOAPActionPlan() {}

void GOAPActionPlan::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("is_finished"), &GOAPActionPlan::is_finished);
    ClassDB::bind_method(D_METHOD("is_empty"), &GOAPActionPlan::is_empty);
    ClassDB::bind_method(D_METHOD("length"), &GOAPActionPlan::length);

    ClassDB::bind_method(D_METHOD("reset"), &GOAPActionPlan::reset);
    ClassDB::bind_method(D_METHOD("execute", "ws", "dt"), &GOAPActionPlan::execute);
}

void GOAPActionPlan::reset()
{
    goal_index = -1;
    action_sequence.clear();
    current_action_index = -1;
    need_start_action = true;
    finished = false;
}

void GOAPActionPlan::execute(WorldState *ws, float dt)
{
    if (ws == nullptr || planner == nullptr)
    {
        godot::print_error("GOAPActionPlan::execute: ws or planner is nullptr");
        return;
    }

    if (!planner->is_goal_valid(goal_index))
    {
        godot::print_error("GOAPActionPlan::execute: Invalid goal index {}", goal_index);
        return;
    }

    if (is_finished() || is_empty())
        return;

    if (!planner->is_action_valid(current_action_index))
        return;

    GOAPPlanner::Action *action = planner->get_action(current_action_index);
    action->sync_world_state_variable(ws);

    if (need_start_action)
    {
        action->start();
        need_start_action = false;
    }

    action->update(dt);

    if (action->is_done())
    {
        action->end();
        action->apply_effect(ws);
        current_action_index++;
        need_start_action = true;
    }

    if (current_action_index >= action_sequence.size())
    {
        current_action_index = action_sequence.size() - 1;
        finished = true;
        GOAPPlanner::Goal *goal = planner->get_goal(goal_index);
        goal->apply_effect(ws);
    }
}