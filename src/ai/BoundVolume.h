#ifndef BOUND_VOLUME_H
#define BOUND_VOLUME_H

//todo: proper refactor of BoundVolume -> BoundVolume2d
//    : modernize
//    Circ2d -> Circ2d : BV2d
//    AABB2d -> AABB2d : BV2d

#include <glm/glm.hpp>
#include <vector>

class Circ;
class Rect;

//either a cylindrical BV or an axis-aligned rectangular BV.
class BoundVolume {
public:
    glm::vec2 _o;//origin
    virtual bool collides(glm::vec2 p) = 0;
    virtual bool line_of_sight(glm::vec2 a, glm::vec2 b, 
        glm::vec2 Lab, float len2) = 0;
    virtual std::vector<BoundVolume*> minkowski_sum(BoundVolume*) = 0;
protected:
    BoundVolume(glm::vec2 o);
    BoundVolume();
};


class Rect : public BoundVolume {
public:
    Rect();
    Rect(glm::vec2 o, float w, float h);
    float _h;//height
    float _w;//width
    bool collides(glm::vec2 p);
    bool line_of_sight(glm::vec2 a, glm::vec2 b, glm::vec2 Lab, float len2);
    std::vector<BoundVolume*> minkowski_sum(BoundVolume*);
    std::vector<BoundVolume*> minkowski_sum_(Rect* b);
    std::vector<BoundVolume*> minkowski_sum_(Circ* b);
private:
    /* broken : but it asks if the lines cross */
    bool lines_collide(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4);
    /*
    axis = 0: the line is x = val {oValLo < y < oValHi} // vertical
    axis = 1: the line is y = val {oValLo < x < oValHi} // horizontal
    asks if pp1->pp2 crosses the above axial-line
    */
    bool line_axial_line_collide(glm::vec2 pp1, glm::vec2 pp2, 
        float val, int axis, float oValLo, float oValHi);
};

class Circ : public BoundVolume {
public:
    Circ();
    Circ(glm::vec2 o, float r);
    float _r;//radius
    bool collides(glm::vec2 p);
    bool line_of_sight(glm::vec2 a, glm::vec2 b, glm::vec2 Lab, float len2);
    std::vector<BoundVolume*> minkowski_sum(BoundVolume*);
    std::vector<BoundVolume*> minkowski_sum_(Rect* b);
    std::vector<BoundVolume*> minkowski_sum_(Circ* b);
};

#endif//BOUND_VOLUME_H
