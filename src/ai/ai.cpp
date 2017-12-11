#include "ai.h"
#include "Pool.h"

namespace ai {
Cspace2d* std_cspace;
PRM* std_prm;
BVH* static_bvh;
BVH* dynamic_bvh;

static void rebuild_sbvh(std::vector<Entity*> statics) {
    static_bvh = new BVH(statics);
}
static void rebuild_dbvh(std::vector<Entity*> dynamics) {
    dynamic_bvh = new BVH(dynamics);
}

//todo: figure out how to handle varying size Cspace
void init() {
    static const float root2 = static_cast<float>(sqrt(2));
    std::vector<Entity*> statics;
    std::vector<Entity*> dynamics;
    POOL.for_<BoundVolume*>([&](BoundVolume*&, const Entity& e) {
        if (POOL.get<Agent>(e) && POOL.get<Dynamics>(e)) {
            dynamics.push_back(&const_cast<Entity&>(e));
        } else {
            statics.push_back(&const_cast<Entity&>(e));
        }
    });

    rebuild_sbvh(statics);
    rebuild_dbvh(dynamics);

    if (dynamics.size() > static_cast<size_t>(0)) {
        //cspace
        std::vector<BoundVolume*> obs_bv;
        for (Entity* s : statics) {
            obs_bv.push_back(*POOL.get<BoundVolume*>(*s));
        }
        std::unique_ptr<Cspace2d> cs(new Cspace2d(obs_bv, &**POOL.get<BoundVolume*>(*dynamics[0])));
        std_cspace = cs.get();

        //prm
        glm::vec2 center_2d(0, 0);//org
        glm::vec2 dim(50/2, 50/2);//w,h
        dim *= 1;//cellsize
        std_prm = new PRM(
            std::move(cs),
            root2 * 12.f,
            0.f,
            glm::vec2(3.f, 3.f),
            1,
            center_2d - dim,
            center_2d + dim,
            .1f);//1.f

        //planners
        for (Entity* e : dynamics) {
            auto& a = *POOL.get<Agent>(*e);
            a.num_done = 0;
            a.cspace = std_cspace;
            a.prm = std_prm;
            a.local_goal = glm::vec2(0, 0);
            a.plan = nullptr;
        }
    }
}

//move to AI/planner --- this is a force-based LMP
void update_agents() {
    std::vector<Entity*> statics;
    std::vector<Entity*> dynamics;
    POOL.for_<BoundVolume*>([&](BoundVolume*&, const Entity& e) {
        if (POOL.get<Agent>(e) && POOL.get<Dynamics>(e)) {
            dynamics.push_back(&const_cast<Entity&>(e));
        } else {
            statics.push_back(&const_cast<Entity&>(e));
        }
    });

    //probably very slow
    delete static_bvh;
    rebuild_sbvh(statics);
    //TODO: fix PRM

    delete dynamic_bvh;
    rebuild_dbvh(dynamics);

    for (Entity* e : dynamics) {
        auto& a = *POOL.get<Agent>(*e);
        if (!a.has_plan() || (a.done() && (a.start != a.final_goal))) {
            GMP::plan_one(a);
        }

        glm::vec2 f2d = LMP::calc_sum_force(e, static_bvh, dynamic_bvh,
            statics, dynamics);
        POOL.get<Dynamics>(*e)->force += glm::vec3(f2d.x, 0, f2d.y);
    }
}
}//
