#include "WorldState.h"
#include "godot_cpp/core/print_string.hpp"
#include <godot_cpp/core/class_db.hpp>

WorldState::WorldState() : is_snapshot(false), master_world_state(nullptr) {}

void WorldState::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("snapshot_from", "src"), &WorldState::snapshot_from);
    ClassDB::bind_method(D_METHOD("copy_data_from", "src"), &WorldState::copy_data_from);
    ClassDB::bind_method(D_METHOD("is_valid"), &WorldState::is_valid);
    ClassDB::bind_method(D_METHOD("get_variable_count"), &WorldState::get_variable_count);
    ClassDB::bind_method(D_METHOD("has_variable", "name"), &WorldState::has_variable);
    ClassDB::bind_method(D_METHOD("get_variable_value", "name"), &WorldState::get_variable_value);
    ClassDB::bind_method(D_METHOD("set_variable_value", "name", "value"), &WorldState::set_variable_value);
    ClassDB::bind_method(D_METHOD("add_variable", "name", "value"), &WorldState::add_variable);
}

void WorldState::snapshot_from(const WorldState *src)
{
    if (src == nullptr)
    {
        godot::print_error("src world state is null");
        return;
    }

    data = src->data;
    is_snapshot = true;
    master_world_state = src->is_snapshot ? src->master_world_state : src;
}

void WorldState::copy_data_from(const WorldState *src)
{
    if (src == nullptr)
        return;

    if (data.size() == src->data.size())
    {
        data = src->data;
    }
}

bool WorldState::is_valid() const noexcept
{
    if (is_snapshot && master_world_state == nullptr)
        return false;

    return data.size() == get_metas().size();
}

int WorldState::get_variable_count() const noexcept { return data.size(); }

int WorldState::get_variable_index(const String &name) const noexcept
{
    for (int i = 0; i < get_metas().size(); i++)
    {
        if (get_metas()[i].name == name)
            return i;
    }
    return -1;
}

bool WorldState::has_variable(const String &name) const noexcept { return get_variable_index(name) != -1; }

Variant WorldState::get_variable_value(const String &name) noexcept
{
    int index = get_variable_index(name);
    if (index == -1)
        return Variant();
    return data[index];
}

void WorldState::set_variable_value(const String &name, const Variant &value) noexcept
{
    int index = get_variable_index(name);
    if (index != -1)
        data[index] = value;
}

void WorldState::add_variable(const String &name, const Variant &value)
{
    if (is_snapshot)
        return;

    Meta m = {};
    m.location = data.size();
    m.name = name;

    data.push_back(value);
    metas.push_back(m);
}