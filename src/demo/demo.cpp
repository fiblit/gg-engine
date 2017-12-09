#include "demo.h"
#include "model/CubeMesh.h"
#include "light/PointLight.h"
#include "light/DirLight.h"
#include "util/debug.h"
#include "Pool.h"
#include "render.h"
#include "util/Seeder.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace demo {
/*
static void create_floor(Entity& e, vector<Texture> texs) {
    uint16_t tid = POOL.create<Transform>(Transform(nullptr));
    uint16_t mid = POOL.create<Mesh>(CubeMesh(texs));
    auto& t = *POOL.get<Transform>(tid);
    glm::mat4 scale(50.f);
    scale[1][1] = 1.f;
    scale[3][3] = 1.f;
    t.set_mat(scale);
    t.set_pos(glm::vec3(0, -1, 0));

    POOL.attach<Transform>(e, tid);
    POOL.attach<Mesh>(e, mid);
}
*/

constexpr unsigned NUM_ROBOS = 100;
constexpr float rot_s = sin(1.f/NUM_ROBOS * glm::pi<float>());
constexpr float rot_c = cos(1.f/NUM_ROBOS * glm::pi<float>());
constexpr float rot_robo[4] = {rot_c, -rot_s, rot_s, rot_c};

static void create_robo(Entity& e, vector<Texture> texs, glm::vec2 pos) {
    uint16_t tid = POOL.create<Transform>(Transform(nullptr));
    uint16_t mid = POOL.create<Mesh>(CubeMesh(texs));
    uint16_t did = POOL.create<Dynamics>(Dynamics());
    uint16_t bvid = POOL.create<BoundVolume*>(new Rect(pos, .3f, .3f));
    uint16_t aid = POOL.create<Agent>(Agent());

    auto& t = *POOL.get<Transform>(tid);
    glm::mat4 scale(.3f);
    scale[1][1] = 1.f;
    scale[3][3] = 1.f;
    t.set_mat(scale);

    auto& d = *POOL.get<Dynamics>(did);
    d.pos = glm::vec3(pos.x, 0, pos.y);
    d.mass = 1;

    auto& a = *POOL.get<Agent>(aid);
    a.final_goal = -(glm::make_mat2(rot_robo) * pos);

    POOL.attach<Transform>(e, tid);
    POOL.attach<Mesh>(e, mid);
    POOL.attach<Dynamics>(e, did);
    POOL.attach<BoundVolume*>(e, bvid);
    POOL.attach<Agent>(e, aid);
}

void init() {
    string pwd(PROJECT_SRC_DIR);
    //so, normally I'd only want one mesh shared amongst many entities, but the
    //renderer does a for_<Mesh> so I can't do that.
     vector<Texture> textures = {
        {render::create_tex(pwd + "/res/container2.png"), Texmap::diffuse},
        {render::create_tex(pwd + "/res/container2_specular.png"), Texmap::specular}
    };

    //Entity& floors = POOL.spawn_entity();
    //create_floor(floors, {});

    Entity* robos[NUM_ROBOS];
    for (unsigned i = 0; i < NUM_ROBOS; ++i) {
        robos[i] = &POOL.spawn_entity();
        float rad = static_cast<float>(i)/NUM_ROBOS * (2.f * glm::pi<float>());
        create_robo(*robos[i], textures, 10.f*glm::vec2(cos(rad), sin(rad)));
    }

    render::dir_lights.push_back(make_unique<DirLight>());
    render::dir_lights.back()->dir(glm::vec3(-1, -1, -1));
    render::dir_lights.back()->ambient(glm::vec3(.1f));
    render::dir_lights.back()->diffuse(glm::vec3(1.f));
    render::dir_lights.back()->specular(glm::vec3(1.f));

    Seeder s;
    typedef uniform_real_distribution<float> UFD;
    UFD x_dist(-50, 50);
    UFD y_dist(3, 10);
    UFD z_dist(-50, 50);
    UFD tweak(-.1f, .1f);
    for (unsigned i = 0; i < 8; ++i) {
        render::point_lights.push_back(make_unique<PointLight>());
        render::point_lights.back()->pos(glm::vec3(
            x_dist(s.gen()), y_dist(s.gen()), z_dist(s.gen())));
        render::point_lights.back()->att_to_dist(100);
        render::point_lights.back()->ambient(glm::vec3(0.f));
        render::point_lights.back()->diffuse(glm::vec3(.5f)
            + glm::vec3(tweak(s.gen()), tweak(s.gen()), tweak(s.gen())));
        render::point_lights.back()->specular(glm::vec3(1.f));
    }
}

void run(double dt, double time) {
    UNUSED(dt, time);
    POOL.for_<Agent>([](Agent& ai, Entity& e) {
        UNUSED(ai, e);
        if (ai.done()) {
            ai.final_goal = -(glm::make_mat2(rot_robo) * ai.start);
        }
    });
}
}
