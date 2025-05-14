#ifndef WORLD_STATE_ASSET_H
#define WORLD_STATE_ASSET_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class WorldStateEntryAsset : public Resource
{
    GDCLASS(WorldStateEntryAsset, Resource)

public:
    enum SupportedType
    {
        Int,
        Float,
        Bool
    };

protected:
    static void _bind_methods();

private:
    String name;
    SupportedType supportedType;
};

class WorldStateAsset : public Resource
{
    GDCLASS(WorldStateAsset, Resource)

private:
    String data;
    int value;

protected:
    static void _bind_methods();

public:
    void set_data(const String &p_data);
    String get_data() const;

    void set_value(int p_value);
    int get_value() const;
};

#endif