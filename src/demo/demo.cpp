#include "demo.h"
#include "model/CubeMesh.h"
#include "light/PointLight.h"
#include "util/debug.h"
#include "Pool.h"
#include "render.h"

#include <string>
#include <vector>

using namespace std;

namespace demo {
void init() {
    string pwd(PROJECT_SRC_DIR);

    for (int i = 0; i < 3; ++i) {
        POOL.spawn_entity();
    }

    vector<Texture> textures = {
        {render::create_tex(pwd + "/res/container2.png"), Texmap::diffuse},
        {render::create_tex(pwd + "/res/container2_specular.png"), Texmap::specular}
    };

    float i = 0;
    POOL.for_entity([&textures, &i](Entity& e){
        //so, normally I'd only want one mesh shared amongst many entities, but the
        //renderer does a for_<Mesh> so I can't do that.
        uint16_t mid = POOL.create<Mesh>(CubeMesh(textures));
        uint16_t tid = POOL.create<Transform>(Transform(nullptr));
        uint16_t did = POOL.create<Dynamics>(Dynamics());
        auto& d = *POOL.get<Dynamics>(did);
        d.pos = glm::vec3(0, i/2.f, 3.f+2.f*i);
        d.vel = glm::vec3(0, 0, i);
        d.force = glm::vec3(0, 0, -i);
        d.mass = 1;
        ++i;
        //d.vel = glm::vec3(0, 0, i/10);
        POOL.attach<Transform>(e, tid);
        POOL.attach<Mesh>(e, mid);
        POOL.attach<Dynamics>(e, did);
    });

    render::point_lights.push_back(make_unique<PointLight>());
    render::point_lights.back()->pos(glm::vec3(0, 3, 3));
    render::point_lights.back()->att_to_dist(100);
    render::point_lights.back()->ambient(glm::vec3(0.1, 0.1, 0.1));
    render::point_lights.back()->diffuse(glm::vec3(1, 0.5, 0.5));
    render::point_lights.back()->specular(glm::vec3(1, 1, 1));
}

void run(double dt, double time) {
    UNUSED(dt, time);
}
}
