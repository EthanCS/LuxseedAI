#ifndef WORLD_STATE_ASSET_H
#define WORLD_STATE_ASSET_H

#include "godot_cpp/core/binder_common.hpp"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class WorldStateEntryAsset : public Resource
{
    GDCLASS(WorldStateEntryAsset, Resource)

public:
    enum SupportedType : unsigned int
    {
        Int,
        Float,
        Bool
    };

private:
    String name;
    SupportedType supportedType;
    Variant value;

protected:
    static void _bind_methods();

    bool _set(const StringName &p_name, const Variant &p_value);
    bool _get(const StringName &p_name, Variant &r_ret) const;
    void _get_property_list(List<PropertyInfo> *p_list) const;

    void reset_value();

public:
    WorldStateEntryAsset();

    String get_name() const;
    void set_name(const String &p_name);

    SupportedType get_supported_type() const;
    void set_supported_type(SupportedType p_type);

    Variant get_value() const;
    void set_value(const Variant &p_value);
};

VARIANT_ENUM_CAST(WorldStateEntryAsset::SupportedType);

class WorldStateAsset : public Resource
{
    GDCLASS(WorldStateAsset, Resource)

private:
    TypedArray<WorldStateEntryAsset> entries;

protected:
    static void _bind_methods();

public:
    WorldStateAsset();

    TypedArray<WorldStateEntryAsset> get_entries() const { return entries; }
    void set_entries(const TypedArray<WorldStateEntryAsset> &p_entries) { entries = p_entries; }
};

#endif