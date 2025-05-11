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
        int KeyID;
        int Location;
        String Name;
    };

private:
    std::vector<Variant> data;
    std::vector<Meta> metas;
    std::map<int, int> keyIndexMapping;
    bool bSnapshot;
    const WorldState *mainWorldState;

private:
    inline const std::vector<Meta> &GetMetas() const noexcept { return bSnapshot ? mainWorldState->metas : metas; }
    inline const std::map<int, int> &GetKeyIndexMapping() const noexcept
    {
        return bSnapshot ? mainWorldState->keyIndexMapping : keyIndexMapping;
    }

protected:
    static void _bind_methods();

public:
    WorldState();

    void SnapshotFrom(const WorldState *src);
    void CopyDataFrom(const WorldState *src);

    bool IsValid() const noexcept;
    int GetKeyID(const String &name) const;
    int GetVariableCount() const noexcept;
    int HasVariable(int KeyID) const;
    Variant GetVariable(int KeyID) noexcept;
    void SetVariable(int KeyID, const Variant &Data) noexcept;
    void AddVariable(int KeyID, const String &Name, const Variant &Data);
};

#endif