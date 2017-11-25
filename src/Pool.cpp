#include "Pool.h"

using namespace std::experimental;

template <typename T>
DataTable::DataTable() :
    _table(),
    _count(0) {
}

template <typename T>
uint16_t DataTable<T>::create(T&& t) {
    //nothing should ever be assigned 0.
    ++_count;
    _table[_count] = std::move(t); 
    return _count;
}

template <typename T>
optional<T&> DataTable<T>::get(uint16_t id) {
    if (id == 0) {
        return nullopt;
    }
    return _table[id];
}

Pool::Pool() :
    _entity_table(),
    _transform_table(),
    _mesh_table(),
    _dynamics_table(),
    _bound_volume_table(),
    _agent_table() {
}

const Entity& Pool::spawn_entity() {
    uint16_t eid = _entity_table.create(Entity(_entity_table._count + 1));
    return *_entity_table.get(eid);
}

//this WILL be slow. :(
void Pool::all_sync() {
    for_entity([this](const Entity& e){
        auto t = get<Transform>(e.comp[0]);
        auto d = get<Dynamics>(e.comp[2]);
        auto bv = get<BoundVolume>(e.comp[3]);
        auto a = get<Agent>(e.comp[4]);
        if (d != nullopt) {
            if (t != nullopt) {
                t->set_pos(d->pos);
            }
        }

        if (t != nullopt) {
            auto p2d = glm::vec2(t->pos().x, t->pos().z);
            if (bv != nullopt) {
                bv->_o = p2d;
            }
            if (a != nullopt) {
                a->start = p2d;
            }
        }
    });
}

void Pool::for_entity(std::function<void(const Entity& )> f) {
    for (auto& e : _entity_table._table) {
        f(e.first);
    }
}

template <typename T> uint16_t Pool::create() {return 0;}
template <typename T> optional<T&> Pool::get(uint16_t) {return nullopt;}
template <typename T> void Pool::attach(const Entity&, uint16_t) {}
template <typename T> void Pool::for_(std::function<void(const T&)>) {}

//what follows is painfully repetitive. Didn't know a better way to do it. :(
template<>
uint16_t Pool::create<Transform>(Transform&& t) {
    return _transform_table.create(t);
}
template<>
optional<Transform&> Pool::get<Transform>(uint16_t id) {
    return _transform_table.get(id);
}
template<>
void Pool::attach<Transform>(const Entity& e, uint16_t id) {
    //transform components go into the 0th reverse index.
    e.comp[0] = id;
}
template<>
void Pool::for_<Transform>(std::function<void(const Transform&)> f) {
    for (auto& t : _transform_table._table) {
        f(t.first);
    }
}

template<>
uint16_t Pool::create<Mesh>(Mesh&& m) {
    return _mesh_table.create(m);
}
template<>
optional<Mesh&> Pool::get<Mesh>(uint16_t id) {
    return _mesh_table.get(id);
}
template<>
void Pool::attach<Mesh>(const Entity& e, uint16_t id) {
    //mesh components go into the 1th reverse index.
    e.comp[1] = id;
}
template<>
void Pool::for_<Mesh>(std::function<void(const Mesh&)> f) {
    for (auto& m : _mesh_table._table) {
        f(m.first);
    }
}

template<>
uint16_t Pool::create<Dynamics>(Dynamics&& d) {
    return _dynamics_table.create(d);
}
template<>
optional<Mesh&> Pool::get<Dynamics>(uint16_t id) {
    return _dynamics_table.get(id);
}
template<>
void Pool::attach<Dynamics>(const Entity& e, uint16_t id) {
    //mesh components go into the 1th reverse index.
    e.comp[2] = id;
}
template<>
void Pool::for_<Dynamics>(std::function<void(const Dynamics&)> f) {
    for (auto& d : _dynamics_table._table) {
        f(d.first);
    }
}

template<>
uint16_t Pool::create<BoundVolume>(BoundVolume&& d) {
    return _bound_volume_table.create(d);
}
template<>
optional<BoundVolume&> Pool::get<BoundVolume>(uint16_t id) {
    return _bound_volume_table.get(id);
}
template<>
void Pool::attach<BoundVolume>(const Entity& e, uint16_t id) {
    //mesh components go into the 1th reverse index.
    e.comp[3] = id;
}
template<>
void Pool::for_<BoundVolume>(std::function<void(const BoundVolume&)> f) {
    for (auto& bv : _bound_volume_table._table) {
        f(bv.first);
    }
}

template<>
uint16_t Pool::create<Agent>(Agent&& d) {
    return _agent_table.create(d);
}
template<>
optional<Agent&> Pool::get<Agent>(uint16_t id) {
    return _agent_table.get(id);
}
template<>
void Pool::attach<Agent>(const Entity& e, uint16_t id) {
    //mesh components go into the 1th reverse index.
    e.comp[4] = id;
}
template<>
void Pool::for_<Agent>(std::function<void(const Agent&)> f) {
    for (auto& a : _agent_table._table) {
        f(a.first);
    }
}
