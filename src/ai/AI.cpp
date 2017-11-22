#include "AI.h"

namespace ai {
Cspace2d* std_cspace;
PRM* std_prm;
BVH<Object>* static_bvh;
BVH<Agent>* dynamic_bvh;

static void rebuild_sbvh(std::vector<Object*> statics) {
    std::vector<std::pair<BoundVolume*, Object*>> bv_static;
    for (Object* st : statics) {
        bv_static.push_back({st->_bv, st});
    }
    static_bvh = new BVH<Object>(bv_static);
}
static void rebuild_dbvh(std::vector<Agent*> dynamics) {
    std::vector<std::pair<BoundVolume*, Agent*>> bv_dynamic;
    for (Agent* d : dynamics) {
        bv_dynamic.push_back({d->_bv, d});
    }
    dynamic_bvh = new BVH<Agent>(bv_dynamic);
}

//todo: figure out how to handle varying size Cspace
void init(std::vector<Agent*> dynamics, std::vector<Object*> statics) {
    static const float root2 = static_cast<float>(sqrt(2));

    rebuild_sbvh(statics);
    rebuild_dbvh(dynamics);
    //for (Object* o : statics) {
    //    o->ai.method = ai_comp::Planner::NONE;
    //}

    if (dynamics.size() > static_cast<size_t>(0)) {
        //cspace
        std::vector<BoundVolume*> obs_bv;
        for (Object* s : statics)
            obs_bv.push_back(s->_bv);
        std::unique_ptr<Cspace2d> cs(new Cspace2d(obs_bv, dynamics[0]->_bv));
        std_cspace = cs.get();

        //prm
        glm::vec2 center_2d(0, 0);//org
        glm::vec2 dim(20/2, 20/2);//w,h
        dim *= 1;//cellsize
        std_prm = new PRM(
            std::move(cs),
            root2,
            0.f,
            glm::vec2(1.f, 1.f),
            1,
            center_2d - dim,
            center_2d + dim,
            .1f);//1.f

        //planners
        for (Agent* o : dynamics) {
            o->_ai->plan = new PointPath();
            o->_ai->num_done = 0;
            o->_ai->cspace = std_cspace;
            o->_ai->prm = std_prm;
            o->_ai->local_goal = glm::vec2(0, 0);
        }
    }
}

//move to AI/planner --- this is a force-based LMP
void update_agents(std::vector<Object*> statics, std::vector<Agent*> dynamics) {
    //if (static_bvh->size() != statics.size()) {
    delete static_bvh;//probably very slow
    rebuild_sbvh(statics);
        //TODO: update PRM
    //}
    delete dynamic_bvh;
    rebuild_dbvh(dynamics);
    //std::vector<Object *> leaders;
    //for (Object * d : dynamics)
    //    if (d->ai.method == ai_comp::Planner::LEAD)
    //        leaders.push_back(d);
    for (Agent* d : dynamics) {
        glm::vec2 f2d = LMP::calc_sum_force(d, static_bvh, dynamic_bvh,
            statics, dynamics);
        d->_dyn->force += glm::vec3(f2d.x, 0, f2d.y);
    }
}
}//
