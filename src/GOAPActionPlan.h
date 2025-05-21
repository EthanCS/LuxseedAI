#ifndef GOAP_ACTION_PLAN_H
#define GOAP_ACTION_PLAN_H

#include <algorithm>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <vector>

#include "GOAPActionBehaviour.h"
#include "GOAPPlanner.h"
#include "WorldState.h"

using namespace godot;

class GOAPActionPlan : public Object
{
    GDCLASS(GOAPActionPlan, Object)

private:
    GOAPPlanner *planner;
    int goal_index;
    std::vector<int> action_sequence;
    int current_action_index;
    bool need_start_action;
    bool finished;

protected:
    static void _bind_methods();

public:
    GOAPActionPlan();

    bool is_finished() const noexcept { return finished; }
    bool is_empty() const noexcept { return action_sequence.empty(); }
    int length() const noexcept { return action_sequence.size(); }

    void reset();
    void execute(WorldState *ws, float dt);

public:
    void swap(int i, int j)
    {
        int temp = action_sequence[i];
        action_sequence[i] = action_sequence[j];
        action_sequence[j] = temp;
    }

    GOAPPlanner *get_planner() const noexcept { return planner; }

    GOAPPlanner::Action *get(int index) noexcept { return planner->get_action(action_sequence[index]); }
    const GOAPPlanner::Action *get(int index) const noexcept { return planner->get_action(action_sequence[index]); }

    void push_back(int action_index) { action_sequence.push_back(action_index); }
    void reverse() { std::reverse(action_sequence.begin(), action_sequence.end()); }
};

#endif