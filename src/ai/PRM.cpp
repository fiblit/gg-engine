#include "PRM.h"
#include "../util/Seeder.h"
#include <random>

using namespace std;

unique_ptr<Points> PRM::nearby(NodeId source, float threshold) {
    unique_ptr<Points> neighbors(new Points());
    glm::vec2 from = *_roadmap->data(source);
    _roadmap->for_vertex([&](NodeId v){
        glm::vec2 test = *_roadmap->data(v);
        if (glm::distance(from, test) < threshold) {
            //push the close enough node onto the neighbors list
            neighbors->insert(v);
        }
    });
    return neighbors;
}

void PRM::connect() {
    _roadmap->for_vertex([&](NodeId v) {
        unique_ptr<Points> near(nearby(v, 5.f));

        //connect nearby to v
        for (NodeId i : *near) {
            //if the nearby points are within line of sight
            if (_cspace->line_of_sight(*_roadmap->data(v), *_roadmap->data(i))) {
                //directed because we'll traverse the other side
                _roadmap->add_dir_edge(v, i); 
            }
        }
    });
}

void PRM::sample_space() {
    Seeder s;
    uniform_real_distribution<float> std(-0.5f, 0.5f);

    int sample_count = 1;
    float b = 2.8f;
    float nudge = .2f;
    glm::vec2 dims(20, 20);
    glm::vec2 radius(dims.x - b, dims.y - b);
    radius /= 2;

    //for each bin
    for (float x = -radius.x; x < radius.x; x += b) {
        for (float y = -radius.y; y < radius.y; y += b) {
            //for multiple samples per bin
            for (int i = 0; i < sample_count; ++i) {
                glm::vec2 sample(std(s.gen())*b + x, std(s.gen())*b + y);
                //nudge until out of something; could theoretically take a while
                while (_cspace->collides(sample)) {
                    sample += nudge * glm::vec2(std(s.gen()), std(s.gen()));
                    sample = glm::clamp(sample, -radius, radius);
                }
                _roadmap->add_vertex(sample);
            }
        }
    }
}

PRM::PRM(glm::vec2 start, glm::vec2 goal, unique_ptr<Cspace2d> cspace) 
        : _cspace(std::move(cspace)) {
    _roadmap = std::make_unique<Graph<glm::vec2>>();

    sample_space();
    _roadmap->add_vertex(start);
    _roadmap->add_vertex(goal);
    connect();
}

Cspace2d::Cspace2d(vector<BoundVolume*> obs, BoundVolume* agent) {
    _obstacles = vector<BoundVolume*>();

    for (BoundVolume* o : obs) {
        vector<BoundVolume*> ms = agent->minkowski_sum(o);
        _obstacles.insert(_obstacles.end(), ms.begin(), ms.end());
    }
}

bool Cspace2d::collides(glm::vec2 p) {
    for (BoundVolume* bv : _obstacles) {
        if (bv->collides(p)) {
            return true;//HIT
        }
    }
    return false;//MISS
}

bool Cspace2d::line_of_sight(glm::vec2 a, glm::vec2 b) {
    glm::vec2 Lab;
    Lab.x = b.x - a.x;
    Lab.y = b.y - a.y;
    float len2 = glm::dot(Lab, Lab);

    for (BoundVolume* bv : _obstacles) {
        if (!bv->line_of_sight(a, b, Lab, len2)) {
            return false;//HIT
        }
    }
    return true;//MISS
}
