
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
#include <iostream>
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
    stats s (imIn);
    edge = imIn.width();
    root = buildTree(s, {0,0}, log2(edge));
}

quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    Node* node = new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));
    // cout<<dim;

    if (dim == 0) {
        // cout<<"哈";
        node->NW = nullptr;
        node->NE = nullptr;
        node->SE = nullptr;
        node->NE = nullptr;
    } else {
        int side = pow(2, dim) / 2;
        node->NW = buildTree(s, ul, dim-1);
        node->NE = buildTree(s, {ul.first+side, ul.second}, dim-1);
        node->SE = buildTree(s, {ul.first+side, ul.second+side}, dim-1);
        node->NE = buildTree(s, {ul.first, ul.second+side}, dim-1);
    }

    return node;
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
