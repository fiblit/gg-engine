#ifndef POOL_H
#define POOL_H

#include "Entity.h"
#include "Transform.h"
#include "ai/Agent.h"
#include "ai/BoundVolume.h"
#include "ai/Dynamics.h"
#include "model/Mesh.h"
#include <experimental/optional>
#include <functional>
//TODO: get rid of unordered_map here, to rushed to do it right right now.
#include <unordered_map>
#include <cstdint>

const unsigned N_COMP = 5;

template <typename T>
class DataTable {
public:
    DataTable();
    template <typename T>
    uint16_t create(T&&);
    template <typename T>
    std::experimental::optional<T&> get(uint16_t);

    //TODO: get rid of unordered map; and the pointers if possible.
    //overusing new is very very very bad
    std::unordered_map<uint16_t, T> _table;
    uint16_t _count;
};

class Pool {
public:
    Pool();

    const Entity& spawn_entity();
    void for_entity(std::function<void(const Entity&)>);
    //fix discrepancies between components for all entities
    void all_sync();

    //T must be a component type
    template <typename T>
    uint16_t create(T&& = T());
    //T must be a component type
    template <typename T>
    std::experimental::optional<T&> get(uint16_t);
    //T must be a component type
    template <typename T>
    void attach(const Entity&, uint16_t);

    template <typename T>
    void for_(std::function<void(const T&));
private:
    DataTable<Entity> _entity_table;
    //A 0 entry = 'null' entity
    DataTable<Transform> _transform_table;
    DataTable<Mesh> _mesh_table;
    DataTable<Dynamics> _dynamics_table;
    DataTable<BoundVolume> _bound_volume_table;
    DataTable<Agent> _agent_table;
};

#endif//POOL_H
