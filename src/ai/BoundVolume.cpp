#include "BoundVolume.h"

Rect::Rect() 
    : BoundVolume(glm::vec2(0, 0), volume_type::RECT), _w(0), _h(0) {}
Rect::Rect(glm::vec2 o, float w, float h)
    : BoundVolume(o, volume_type::RECT), _w(w), _h(h) {}

Circ::Circ() 
    : BoundVolume(glm::vec2(0, 0), volume_type::CIRC), _r(0) {}
Circ::Circ(glm::vec2 o, float r) 
    : BoundVolume(o, volume_type::CIRC), _r(r) {}

BoundVolume::~BoundVolume() {}
BoundVolume::BoundVolume() : _o(glm::vec2(0, 0)), _vt(volume_type::CIRC) {}
BoundVolume::BoundVolume(glm::vec2 o, volume_type vt) : _o(o), _vt(vt) {}

//TODO: rect/circ collision could be simplified via:
//test if point p is in rect such that p is r along the direction from circle to rect
//note: "along the direction" is not as simple as it sounds, probably not simpler
std::vector<BoundVolume*> Circ::minkowski_sum(BoundVolume* bv) {
    if (_vt == volume_type::CIRC) {
        return minkowski_sum_(dynamic_cast<Circ*>(bv));
    }
    if (_vt == volume_type::RECT) {
        return minkowski_sum_(dynamic_cast<Rect*>(bv));
    }
    return std::vector<BoundVolume*>();
}
std::vector<BoundVolume*> Circ::minkowski_sum_(Circ* b) {
    std::vector<BoundVolume*> bv = {new Circ(b->_o, _r + b->_r)};
    return bv;
}
std::vector<BoundVolume*> Circ::minkowski_sum_(Rect* b) {
    std::vector<BoundVolume*> bv = {
        new Rect(b->_o, b->_w, 2 * _r + b->_h),
        new Rect(b->_o, 2 * _r + b->_w, b->_h),
        new Circ(glm::vec2(b->_o.x + b->_w / 2, b->_o.y + b->_h / 2), _r),
        new Circ(glm::vec2(b->_o.x - b->_w / 2, b->_o.y + b->_h / 2), _r),
        new Circ(glm::vec2(b->_o.x + b->_w / 2, b->_o.y - b->_h / 2), _r),
        new Circ(glm::vec2(b->_o.x - b->_w / 2, b->_o.y - b->_h / 2), _r),
    };
    return bv;
}

std::vector<BoundVolume*> Rect::minkowski_sum(BoundVolume* bv) {
    if (_vt == volume_type::CIRC) {
        return minkowski_sum_(dynamic_cast<Circ*>(bv));
    }
    if (_vt == volume_type::RECT) {
        return minkowski_sum_(dynamic_cast<Rect*>(bv));
    }
    return std::vector<BoundVolume*>();
}

std::vector<BoundVolume*> Rect::minkowski_sum_(Rect* b) {
    std::vector<BoundVolume*> bv = {new Rect(b->_o, _w + b->_w, _h + b->_h)};
    return bv;
}
std::vector<BoundVolume*> Rect::minkowski_sum_(Circ* b) {
    std::vector<BoundVolume*> bv = {
        new Rect(b->_o, _w, 2 * b->_r + _h),
        new Rect(b->_o, 2 *b->_r + _w, _h),
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
    //we don't use is_collision because we use a lot of these values again
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

float Circ::intersect(glm::vec2 bo, glm::vec2 v) {
    //min t = - (u.v) +- sqrt((u.v)^. - (v.v) (u.u - r^))/(v.v)
    glm::vec2 u = bo - _o;
    float c = glm::dot(u, u) - _r * _r;
    if (c < 0)//inside 
        return 0;

    float b = glm::dot(u, v);
    float a = glm::dot(v, v);

    float d = b*b - a*c;
    if (d <= 0)//no intersect, forward or backwards
        return std::numeric_limits<float>::max();

    float t = (-b - static_cast<float>(sqrt(d))) / a;
    if (t < 0)//intersction is behind ray
        return std::numeric_limits<float>::max();

    return t;
}

bool Rect::collides(glm::vec2 p) {
    return fabs(p.x - _o.x) <= _w / 2
        && fabs(p.y - _o.y) <= _h / 2;
}

bool Rect::line_of_sight(glm::vec2 a, glm::vec2 b, glm::vec2 /*Lab*/, float /*len2*/) {
    //float t = intersect(b, Lab/sqrt(len2));//the intersect for axis aligned is actually pretty fast
    //return t*t > len2;

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
//deprecated
bool Rect::line_segs_collide(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4) {
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
bool Rect::line_axial_line_collide(glm::vec2 pp1, glm::vec2 pp2, float val,
        int axis, float oValLo, float oValHi) {
    glm::vec3 l = glm::cross(glm::vec3(pp1, 1), glm::vec3(pp2, 1));
    //ax+by+circ = 0

    //vertical
    if (axis == 0) {// (1/val)*x + 0*y - 1 = 0 // x = val
        float yint = (-l[0] * val - l[2]) / l[1];
        //val line hits lineseg
        return ((pp1.x <= val && val <= pp2.x) || (pp2.x <= val && val <= pp1.x))
            //intersection on axial segment
            && (oValLo <= yint && yint <= oValHi);
    }
    //horizontal
    else {//if (axis == 1) {// 0x + (1/val)*y - 1 = 0 // y =val
        float xint = (-l[1] * val - l[2]) / l[0];
        //axial line hits line seg
        return ((pp1.y <= val && val <= pp2.y) || (pp2.y <= val && val <= pp1.y))
            //intersection on axial segment
            && (oValLo <= xint && xint <= oValHi);
    }
}

static float ray_axial_line_intersect(glm::vec2 po, glm::vec2 pv, float val,
        int axis, float oValLo, float oValHi){
    //https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/
    glm::vec2 q, s;
    if (axis == 0) {//vert 
        q = glm::vec2(val, oValLo);
        s = glm::vec2(0, oValHi - oValLo);
    }
    else {
        q = glm::vec2(oValLo, val);
        s = glm::vec2(oValHi - oValLo, 0);
    }
    glm::vec2 v1 = po - q;
    glm::vec2 v2 = s - q;
    glm::vec2 v3(-pv.y, pv.x);
    float dot = glm::dot(v2, v3);
    if (fabs(dot) > 0) {
        float t1 = (v2.x * v1.y - v2.y * v1.x) / dot;
        float t2 = glm::dot(v1, v3) / dot;
        if (t1 >= 0 && 0 <= t2 && t2 <= 1)
            return t1;
        else
            return std::numeric_limits<float>::max();
    }
    else
        return std::numeric_limits<float>::max();


    //http://stackoverflow.com/questions/14307158/ ...
    //how-do-you-check-for-intersection-between-a-line-segment-and-a-line-ray-emanatin
    //http://stackoverflow.com/questions/563198/ ...
    //how-do-you-detect-where-two-line-segments-intersect/565282#565282
    /*glm::vec2 p = po;
    glm::vec2 r = pv;
    //v x w = vx wy − vy wx
    //t = (q − p) × s / (r × s)
    //u = (q − p) × r / (r × s)
    glm::vec2 qp = q - p;
    float rxs = r.x * s.y - r.y * s.x;
    float qpxs = qp.x * s.y - qp.y * s.x;
    float qpxr = qp.x * r.y - qp.y * r.x;
    float e = 0.0000001;
    if (abs(rxs) < e) {
        return std::numeric_limits<float>::max();
    }
    else {
        float t = qpxs / rxs;
        float u = qpxr / rxs;
        if (0 <= t && 0 <= u && u <= 1)
            return t;
        else
            return std::numeric_limits<float>::max();
    }*/
}

//assumes axis alignment
float Rect::intersect(glm::vec2 bo, glm::vec2 v) {
    if (collides(bo))
        return 0;
    
    float left = _o.x - _w / 2;
    float right = _o.x + _w / 2;
    float top = _o.y + _h / 2;
    float bottom = _o.y - _h / 2;

    float t = std::numeric_limits<float>::max();
    float possible = ray_axial_line_intersect(bo, v, left, 0, bottom, top);
    if (possible < t) t = possible;
    possible = ray_axial_line_intersect(bo, v, right, 0, bottom, top);
    if (possible < t) t = possible;
    possible = ray_axial_line_intersect(bo, v, bottom, 1, left, right);
    if (possible < t) t = possible;
    possible = ray_axial_line_intersect(bo, v, top, 1, left, right);
    if (possible < t) t = possible;

    return t;
    /*
    glm::vec2 d_o = bo - _o;

    float t_x_hi = (_w - d_o.x) / v.x;
    float t_y_hi = (_h - d_o.y) / v.y;

    float t_x_lo = (-_w - d_o.x) / v.x;
    float t_y_lo = (-_h - d_o.y) / v.y;

    if (t_x_hi > t_x_lo) {
        float t = t_x_hi;
        t_x_hi = t_x_lo;
        t_x_lo = t_x_hi;
    }
    if (t_y_hi > t_y_lo) {
        float t = t_y_hi;
        t_y_hi = t_y_lo;
        t_y_lo = t_y_hi;
    }

    //no intersection forward or back
    if (t_x_hi < t_y_lo || t_x_lo > t_y_hi)
        return std::numeric_limits<float>::max();
    else {
        float t_lo = (t_x_lo > t_y_lo ? t_x_lo : t_y_lo);
        float t_hi = (t_x_hi > t_y_hi ? t_x_hi : t_y_hi);
        if (t_hi < 0)//intersection before ray
            return std::numeric_limits<float>::max();
        else if (t_lo < 0)
            return 0; //intersection current
        else
            return t_lo;
    }
    */
}
