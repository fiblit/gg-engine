#ifndef BVH_H
#define BVH_H

#include "BoundVolume.h"
#include <vector>
#include <algorithm>

//todo: have BVH hold anything, not just one type, so long as that thing has a
//bounding volume
template <class T>
class BVH {
public:
    typedef std::pair<BoundVolume*,T*> BVData;

    BVH() : o(nullptr), right(nullptr) {};
    BVH(std::vector<BVData> objects) {
        this->size_ = objects.size();
        if (objects.size() == 0) {
            this->o = nullptr;
            this->left = nullptr;
            this->right = nullptr;
            return;
        }

        //create two index arrays
        std::vector<Index> sorted_x(objects.size());
        std::vector<Index> sorted_z(objects.size());
        for (size_t i = 0; i < objects.size(); i++)
            //for sorted_a Index is (idx of obj for x, idx of zidx)
            //for sorted_b Index is (idx of obj for z, idx of xidx)
            //so .obj will always grab the object,
            //.oth the other table's index to the same object
            sorted_x[i] = sorted_z[i] = Index(static_cast<uint>(i), 0);

        //sort by dimension
        std::sort(sorted_x.begin(), sorted_x.end(), [objects](Index a, Index b) {
            return objects[a.obj].first->_o.x < objects[b.obj].first->_o.x;
        });
        for (size_t i = 0; i < sorted_x.size(); i++)//connect index tables
            sorted_z[sorted_x[i].obj].oth = static_cast<uint>(i);

        std::sort(sorted_z.begin(), sorted_z.end(), [objects](Index a, Index b) {
            return objects[a.obj].first->_o.y < objects[b.obj].first->_o.y;
        });
        for (size_t i = 0; i < sorted_z.size(); i++)//connect index tables
            sorted_x[sorted_z[i].oth].oth = static_cast<uint>(i);

        construct_(objects, sorted_x, sorted_z);
    }

    ~BVH() {
        if (!is_leaf()) {
            delete left;
            delete right;
        }
    };

    //only use Circ queries please...
    std::vector<BVData> query(BoundVolume* q) {
        std::vector<std::pair<BoundVolume*,T*>> NN;
        if (left == nullptr) {
            return std::vector<std::pair<BoundVolume*,T*>>();
        }
        //we are making terrible assumptions
        query_(dynamic_cast<Circ*>(q), &NN);
        return NN;
    };

private:
    //terrible assumptions
    void query_(Circ* q, std::vector<BVData>* NN) {
        if (is_leaf()) {
            if (o != nullptr) {
                BoundVolume* bv = o->first;
                if (bv != nullptr) {
                    if (bv->_vt == BoundVolume::volume_type::CIRC) {
                        if (circ_circ_collider_(q, static_cast<Circ*>(bv))) {
                            NN->push_back(*o);
                        }
                    }
                    else {
                        if (circ_rect_collider_(q, static_cast<Rect *>(bv))) {
                            NN->push_back(*o);
                        }
                    }
                }
            }
        }
        else {
            if (circ_rect_collider_(q, &aabb)) {
                this->left->query_(q, NN);
                this->right->query_(q, NN);
            }
        }
    };
    //don't use, test later as the sqrt will probably make this slower
    //this would be useful if it is faster, though, since circ/rect is common
    //that would also mean I need to update my BV code.
    bool circ_rect_collider_(Circ* q, Rect* r) {
        glm::vec2 L = r->_o - q->_o;
        L /= sqrt(glm::dot(L, L));
        L *= q->_r;
        L += q->_o;
        return q->collides(r->_o) || r->collides(L);
    };

    bool circ_circ_collider_(Circ* q, Circ* c) {
        glm::vec2 diff = q->_o - c->_o;
        float r = q->_r + c->_r;
        return glm::dot(diff, diff) < r*r;
    };
public:
    bool is_leaf() {
        return right == nullptr;
    }
    size_t size() {
        return this->size_;
    }
    
    union {
        BVH<T>* left;
        BVData* o;
    };
    BVH<T>* right;
    Rect aabb;

private:
    size_t size_;

    typedef unsigned int uint;
    struct Index {
        uint obj;//object/this
        uint oth;//other
        Index() { obj = 0; oth = 0; };
        Index(uint obj_, uint oth_) { this->obj = obj_; this->oth = oth_; }
    };

    void construct_(
            std::vector<BVData> objects,
            std::vector<Index> sorted_x,
            std::vector<Index> sorted_z) {
        assert(sorted_x.size() == sorted_z.size());
        this->size_ = sorted_x.size();
        assert(sorted_x.size() >= 1);
        if (sorted_x.size() == static_cast<size_t>(1)) {
            assert(sorted_x[0].obj == sorted_z[0].obj);
            this->right = nullptr;
            o = &objects[sorted_x[0].obj];
            return;
        }

        //create bounding volume for this level, remembering the min/max in each dim
        float min_x, max_x, min_z, max_z;
        min_x = min_z = std::numeric_limits<float>::max();
        max_x = max_z = -std::numeric_limits<float>::max();
        //tightly fit min_x/max_x/min_z/max_z
        for (size_t i = 0; i < sorted_x.size(); i++) {
            BoundVolume* bv = objects[sorted_x[i].obj].first;
            float dim_x, dim_z;
            if (bv->_vt == BoundVolume::volume_type::RECT) {
                //add a nudge if you want fat BVs
                dim_x = static_cast<Rect*>(bv)->_w/2;
                dim_z = static_cast<Rect*>(bv)->_h/2;
            }
            else {
                dim_x = dim_z = static_cast<Circ*>(bv)->_r;
            }

            float x_lo = bv->_o.x - dim_x;
            float x_hi = bv->_o.x + dim_x;
            float z_lo = bv->_o.y - dim_z;
            float z_hi = bv->_o.y + dim_z;
            if (x_lo < min_x) min_x = x_lo;
            if (x_hi > max_x) max_x = x_hi;
            if (z_lo < min_z) min_z = z_lo;
            if (z_hi > max_z) max_z = z_hi;
        }

        float dx = max_x - min_x;
        float dz = max_z - min_z;
        aabb = Rect(glm::vec2(min_x + dx/2, min_z + dz/2), dx, dz);

        //partition along longer axis, splitting indices equally
        std::vector<Index> x_rhs, x_lhs;
        std::vector<Index> z_rhs, z_lhs;
        if (dx > dz) {
            split_(sorted_x, sorted_z, x_lhs, z_lhs, x_rhs, z_rhs);
        } else {
            split_(sorted_z, sorted_x, z_lhs, x_lhs, z_rhs, x_rhs);
        }

        //probably the slowest part is the new, tbh XD
        left = new BVH();
        left->construct_(objects, x_lhs, z_lhs);
        right = new BVH();
        right->construct_(objects, x_rhs, z_rhs);
    };
    void split_(
            std::vector<Index>& sorted_a,
            std::vector<Index>& sorted_b,
            std::vector<Index>& a_lhs,
            std::vector<Index>& b_lhs,
            std::vector<Index>& a_rhs,
            std::vector<Index>& b_rhs) {
        assert(sorted_a.size() == sorted_b.size());
        uint half = static_cast<uint>(static_cast<double>(sorted_a.size()) / 2.);
        uint half_ceil = static_cast<uint>((static_cast<double>(sorted_a.size()) / 2.) + .5);
        a_lhs = std::vector<Index>(half);
        b_lhs = std::vector<Index>(half);
        a_rhs = std::vector<Index>(half_ceil);
        b_rhs = std::vector<Index>(half_ceil);

        for (size_t i = 0; i < sorted_a.size(); i++) {
            if (i < half) {
                a_lhs[i] = Index(sorted_a[i].obj, std::numeric_limits<unsigned int>::max());
            } else {
                a_rhs[i - half] = Index(sorted_a[i].obj, std::numeric_limits<unsigned int>::max());
            }
        }
        for (uint i = 0, lhs = 0, rhs = 0; i < static_cast<uint>(sorted_b.size()); i++) {
            uint oth_i = sorted_b[i].oth;
            if (oth_i < half) {
                b_lhs[lhs] = Index(sorted_b[i].obj, oth_i);
                assert(oth_i < a_lhs.size());
                a_lhs[oth_i] = Index(a_lhs[oth_i].obj, lhs);
                lhs++;
            }
            else {
                b_rhs[rhs] = Index(sorted_b[i].obj, oth_i - half);
                assert(oth_i - half < a_rhs.size());
                a_rhs[oth_i - half] = Index(a_rhs[oth_i - half].obj, rhs);
                rhs++;
            }
        }
    };
};

//http://www.randygaul.net/2013/08/06/dynamic-aabb-tree/
//to avoid excessive use of the heap and indirection
//also, he mentions the use of an AVL tree to maintain balance
/*
struct BVH_node {
    static const int Null = -1;
    bool is_leaf() {
        return right == Null;
    }
    Rect aabb;
    union {
        int parent;
        int next;
    };
    union {
        struct {
            int left;
            int right;
        };
        Object * o;
    };
    int height;
};
*/
#endif//BVH_H
