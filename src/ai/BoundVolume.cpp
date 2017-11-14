#include "BoundVolume.h"

using namespace std;

BoundVolume::BoundVolume() {}
BoundVolume::BoundVolume(glm::vec2 o) : _o(o) {}

Rect::Rect() {}
Rect::Rect(glm::vec2 o, float w, float h) : BoundVolume(o), _h(h), _w(w) {}

Circ::Circ() {}
Circ::Circ(glm::vec2 o, float r) : BoundVolume(o),_r(r) {}

vector<BoundVolume *> Circ::minkowski_sum(BoundVolume* bv) {
    //I wish there was a way I didn't have to check the types..
    Circ* c = dynamic_cast<Circ*>(bv);
    if (c != nullptr) {
        return minkowski_sum_(c);
    }
    
    Rect * r = dynamic_cast<Rect*>(bv);
    if (r != nullptr) {
        return minkowski_sum_(r);
    }

    return vector<BoundVolume*>();
}
vector<BoundVolume*> Circ::minkowski_sum_(Circ* b) {
    vector<BoundVolume*> bv = {new Circ(b->_o, _r + b->_r)};
    return bv;
}
vector<BoundVolume*> Circ::minkowski_sum_(Rect* b) {
    vector<BoundVolume*> bv = {
        new Rect(b->_o, b->_w, 2 * _r + b->_h),
        new Rect(b->_o, 2 * _r + b->_w, b->_h),
        new Circ(glm::vec2(b->_o.x + b->_w / 2, b->_o.y + b->_h / 2), _r),
        new Circ(glm::vec2(b->_o.x - b->_w / 2, b->_o.y + b->_h / 2), _r),
        new Circ(glm::vec2(b->_o.x + b->_w / 2, b->_o.y - b->_h / 2), _r),
        new Circ(glm::vec2(b->_o.x - b->_w / 2, b->_o.y - b->_h / 2), _r),
    };
    return bv;
}

vector<BoundVolume*> Rect::minkowski_sum(BoundVolume* bv) {
    Circ* c = dynamic_cast<Circ*>(bv);
    if (c != nullptr) {
        return minkowski_sum_(c);
    }

    Rect * r = dynamic_cast<Rect*>(bv);
    if (r != nullptr) {
        return minkowski_sum_(r);
    }

    return vector<BoundVolume*>();
}
vector<BoundVolume*> Rect::minkowski_sum_(Rect* b) {
    vector<BoundVolume*> bv = {new Rect(b->_o, _w + b->_w, _h + b->_h)};
    return bv;
}

vector<BoundVolume*> Rect::minkowski_sum_(Circ * b) {
    vector<BoundVolume*> bv = {
        new Rect(b->_o, _w, 2 * b->_r + _h),
        new Rect(b->_o, 2 * b->_r + _w, _h),
        new Circ(glm::vec2(b->_o.x + _w / 2, b->_o.y + _h / 2), b->_r),
        new Circ(glm::vec2(b->_o.x - _w / 2, b->_o.y + _h / 2), b->_r),
        new Circ(glm::vec2(b->_o.x + _w / 2, b->_o.y - _h / 2), b->_r),
        new Circ(glm::vec2(b->_o.x - _w / 2, b->_o.y - _h / 2), b->_r),
    };
    return bv;
}


bool Circ::collides(glm::vec2 p) {
    glm::vec2 diff = p - _o;
    return glm::dot(diff, diff) <= _r * _r;
}

bool Circ::line_of_sight(glm::vec2 a, glm::vec2 b, glm::vec2 Lab, float len2) {
    glm::vec2 Lbo = _o - b;
    float r2 = _r * _r;
    if (glm::dot(Lbo, Lbo) <= r2) //point b inside circle
        return false; // HIT

    glm::vec2 Lao = _o - a;
    //we don't use isCollision because we use a lot of these values again
    if (glm::dot(Lao, Lao) <= r2) //point a inside circle
        return false; // HIT

    float ang = glm::dot(Lab, Lao);
    glm::vec2 proj = Lab * (ang / len2);
    glm::vec2 rej = Lao - proj;
    float projlen2 = glm::dot(proj, proj);

    if (glm::dot(rej, rej) <= r2 //close enough tangentially
        && 0 <= ang              //point a before circle center
        && projlen2 <= len2)     //point b after circle center
        return false; // HIT

    return true; // else MISS
}

bool Rect::collides(glm::vec2 p) {
    return abs(p.x - _o.x) <= _w / 2
        && abs(p.y - _o.y) <= _h / 2;
}

bool Rect::line_of_sight(glm::vec2 a, glm::vec2 b, glm::vec2, float) {
    float left = _o.x - _w / 2;
    float right = _o.x + _w / 2;
    float top = _o.y + _h / 2;
    float bottom = _o.y - _h / 2;

    return !(line_axial_line_collide(a, b, left, 0, bottom, top)
        || line_axial_line_collide(a, b, right, 0, bottom, top)
        || line_axial_line_collide(a, b, bottom, 1, left, right)
        || line_axial_line_collide(a, b, top, 1, left, right));
}

//TODO: fix, so I can rotate rects
bool Rect::lines_collide(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4) {
    glm::vec2 pp[4] = { p1, p2, p3, p4 };
    glm::vec3 l[2], p[4], x;

    for (int i = 0; i < 4; i++)
        p[i] = glm::vec3(pp[i].x, pp[i].y, 1);

    l[0] = glm::cross(p[0], p[1]);
    l[1] = glm::cross(p[2], p[3]);
    x = glm::cross(l[0], l[1]);
    glm::vec2 px;
    px.x = x.x / x.z;
    px.y = x.y / x.z;

    float len2l1 = (l[0].y * l[0].y) + (l[0].x * l[0].x);
    float len2l2 = (l[1].y * l[1].y) + (l[1].x * l[1].x);

    glm::vec2 pp2x[4];
    for (int i = 0; i < 4; i++)
        pp2x[i] = px - pp[i];
    for (int i = 0; i < 2; i++)
        if (glm::dot(pp2x[i], pp2x[i]) > len2l1)
            return false;//miss
    for (int i = 3; i < 5; i++)
        if (glm::dot(pp2x[i], pp2x[i]) > len2l2)
            return false;//miss

    return true;//must have hit
}

bool Rect::line_axial_line_collide(glm::vec2 pp1, glm::vec2 pp2, 
        float val, int axis, float oValLo, float oValHi) {
    glm::vec3 l = glm::cross(glm::vec3(pp1, 1), glm::vec3(pp2, 1));
    //ax+by+c = 0

    //vertical
    if (axis == 0) {// (1/val)*x + 0*y - 1 = 0 // x = val
        float yint = (-l[0] * val - l[2]) / l[1];
        //val line hits lineseg
        return ((pp1.x <= val && val <= pp2.x) || (pp2.x <= val && val <= pp1.x))
            && (oValLo <= yint && yint <= oValHi); //intersection on axial segment
    }
    //horizontal
    else {//if (axis == 1) {// 0x + (1/val)*y - 1 = 0 // y =val
        float xint = (-l[1] * val - l[2]) / l[0];
        //axis line hits lineseg
        return ((pp1.y <= val && val <= pp2.y) || (pp2.y <= val && val <= pp1.y))
            && (oValLo <= xint && xint <= oValHi); //intersection on axial segment
    }
}
