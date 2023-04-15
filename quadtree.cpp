
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree& other) {
    copy(other);
}
// quadtree assignment operator, given.
quadtree& quadtree::operator=(const quadtree& rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

quadtree::quadtree(PNG& imIn) {
    /* Your code here! */
}

quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    /* Your code here! */
    return nullptr;
}

PNG quadtree::render() const {
    /* Your code here! */
    return PNG();
}

int quadtree::idealPrune(const int leaves) const {
    /* Your code here! */
    return 0;
}

int quadtree::pruneSize(const int tol) const {
    /* Your code here! */
    return 0;
}

void quadtree::prune(const int tol) {
    /* Your code here! */
}

void quadtree::clear() {
    /* your code here */
}

void quadtree::copy(const quadtree& orig) {
    /* your code here */
}
