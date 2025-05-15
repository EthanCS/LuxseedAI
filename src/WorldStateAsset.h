#ifndef WORLD_STATE_ASSET_H
#define WORLD_STATE_ASSET_H

#include "godot_cpp/core/binder_common.hpp"
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
    static constexpr int kMaxBufferSize = 16;

protected:
    static void _bind_methods();

    bool _get(const StringName &p_name, Variant &r_ret) const;
    bool _set(const StringName &p_name, const Variant &p_value);
    void _get_property_list(List<PropertyInfo> *p_list) const;

private:
    String name;
    SupportedType supportedType;
    char buffer[kMaxBufferSize];

    PackedByteArray get_buffer_data() const;
    void set_buffer_data(const PackedByteArray &p_data);

public:
    String get_name() const;
    void set_name(const String &p_name);

    SupportedType get_supported_type() const;
    void set_supported_type(SupportedType p_type);
};

VARIANT_ENUM_CAST(WorldStateEntryAsset::SupportedType);

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