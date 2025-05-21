#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <map>
#include <vector>

using namespace godot;

class WorldState : public Object
{
    GDCLASS(WorldState, Object)

    struct Meta
    {
        int location;
        String name;
    };

private:
    std::vector<Variant> data;
    std::vector<Meta> metas;
    bool is_snapshot;
    const WorldState *master_world_state;

private:
    inline const std::vector<Meta> &get_metas() const noexcept
    {
        return is_snapshot ? master_world_state->metas : metas;
    }

protected:
    static void _bind_methods();

public:
    WorldState();

    void snapshot_from(const WorldState *src);
    void copy_data_from(const WorldState *src);
    bool is_valid() const noexcept;
    int get_variable_count() const noexcept;
    bool has_variable(const String &name) const noexcept;
    Variant get_variable_value(const String &name) noexcept;
    void set_variable_value(const String &name, const Variant &value) noexcept;
    void add_variable(const String &name, const Variant &value);

private:
    int get_variable_index(const String &name) const noexcept;
};

#endif