
// /**
//  *
//  * quadtree (pa3)
//  * quadtree.cpp
//  * This file will be used for grading.
//  *
//  */

// #include "quadtree.h"
// #include <iostream>
// using namespace std;

// // Node constructor, given.
// quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
//     : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// // quadtree destructor, given.
// quadtree::~quadtree() {
//     clear();
// }
// // quadtree copy constructor, given.
// quadtree::quadtree(const quadtree& other) {
//     copy(other);
// }
// // quadtree assignment operator, given.
// quadtree& quadtree::operator=(const quadtree& rhs) {
//     if (this != &rhs) {
//         clear();
//         copy(rhs);
//     }
//     return *this;
// }

// quadtree::quadtree(PNG& imIn) {
//     stats s (imIn);
//     edge = imIn.width();
//     root = buildTree(s, {0,0}, log2(edge));
// }

// quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
//     Node* node = new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));

//     if (dim == 0) {
//         node->NW = nullptr;
//         node->NE = nullptr;
//         node->SE = nullptr;
//         node->NE = nullptr;
//     } else {
//         int side = pow(2, dim) / 2;
//         node->NW = buildTree(s, ul, dim-1);
//         node->NE = buildTree(s, {ul.first+side, ul.second}, dim-1);
//         node->SE = buildTree(s, {ul.first+side, ul.second+side}, dim-1);
//         node->NE = buildTree(s, {ul.first, ul.second+side}, dim-1);
//     }

//     return node;
// }

// PNG quadtree::render() const {
//     PNG result;
//     result.resize(edge, edge);

//     for (int x=0; x<edge; x++) {
//         for (int y=0; y<edge; y++) {
//             result.getPixel(x,y);
//         }
//     }

//     return result;
// }

// int quadtree::idealPrune(const int leaves) const {
//     /* Your code here! */
//     return 0;
// }

// int quadtree::pruneSize(const int tol) const {
//     /* Your code here! */
//     return 0;
// }

// void quadtree::prune(const int tol) {
//     /* Your code here! */
// }

// void quadtree::clear() {
//     /* your code here */
// }

// void quadtree::copy(const quadtree& orig) {
//     /* your code here */
// }


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
    edge = imIn.width();
    stats s(imIn);
    root = buildTree(s, make_pair(0, 0), log2(edge));
}

// quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {

//     Node* currentNode = new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));

    
//     if (dim == 0) {
//         RGBAPixel avgPixel = s.getAvg(ul, dim);
//         double vari = s.getVar(ul, dim);
//         return new Node(ul, dim, avgPixel, vari);
//     }

//     int nDim = dim - 1;
//     int offset = pow(2, nDim);

    
//     currentNode->NW = buildTree(s, ul, nDim);
//     currentNode->NE = buildTree(s, make_pair(ul.first + offset, ul.second), nDim);
//     currentNode->SW = buildTree(s, make_pair(ul.first, ul.second + offset), nDim);
//     currentNode->SE = buildTree(s, make_pair(ul.first + offset, ul.second + offset), nDim);

//     return currentNode;
// }

quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    Node* node = new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));

    if (dim == 0) {
        cout<<" leaf " << "dim: " << dim << "\n";
        node->NW = nullptr;
        node->NE = nullptr;
        node->SE = nullptr;
        node->NE = nullptr;
    } else {
        cout<<" build " << "dim: " << dim << "\n";
        int side = pow(2, dim) / 2;
        node->NW = buildTree(s, ul, dim-1);
        node->NE = buildTree(s, {ul.first+side, ul.second}, dim-1);
        node->SW = buildTree(s, {ul.first, ul.second+side}, dim-1);
        node->SE = buildTree(s, {ul.first+side, ul.second+side}, dim-1);
    }

    return node;
}

PNG quadtree::render() const {
    if (root == nullptr) {
        return PNG();
    }
    int side = pow(2, static_cast<int>(log2(edge)));

    PNG img(side, side);
    renderHelper(img, root);
    return img;
}

void quadtree::renderHelper(PNG& img, Node* node) const {
    if (node == nullptr) {
        return;
    }

    if (node->NW == nullptr && node->NE == nullptr && node->SW == nullptr && node->SE == nullptr) {
        int xBegin = node->upLeft.first;
        int yBegin = node->upLeft.second;
        int blockSize = 1 << node->dim;

        for (int a = xBegin; a < xBegin + blockSize; a++) {
            for (int b = yBegin; b < yBegin + blockSize; b++) {
                *img.getPixel(a, b) = node->avg;
            }
        }
    } else {
        renderHelper(img, node->NW);
        renderHelper(img, node->NE);
        renderHelper(img, node->SW);
        renderHelper(img, node->SE);
    }
}

double quadtree::getMaxVar(Node* node) const {
    if (node == nullptr) {
        return 0;
    }

    double maxVar = node->var;
    double maxVar_NW = max(maxVar, getMaxVar(node->NW));
    double maxVar_NE = max(maxVar, getMaxVar(node->NE));
    double maxVar_SW = max(maxVar, getMaxVar(node->SW));
    double maxVar_SE = max(maxVar, getMaxVar(node->SE));

    return max({maxVar_NW, maxVar_NE, maxVar_SW, maxVar_SE});
}


int quadtree::idealPrune(const int leaves) const {
    int maxxVar = static_cast<int>(getMaxVar(root));
    return idealPruneHelper(0, maxxVar, leaves);
}

int quadtree::idealPruneHelper(const int lower, const int upper, const int target) const {
    if (lower >= upper) {
        return lower;
    }

    int middle = (lower + upper) / 2;
    int size = pruneSize(middle);

    if (size <= target) {
        return idealPruneHelper(lower, middle, target);
    } else {
        return idealPruneHelper(middle + 1, upper, target);
    }
}

int quadtree::pruneSize(const int tol) const {
    return pruneSizeHelper(root, tol);
}

int quadtree::pruneSizeHelper(Node* node, const int tol) const {
    if (node == nullptr) {
        return 0;
    }

    if (prunable(node, tol)) {
        return 1;
    }

    return pruneSizeHelper(node->NW, tol) + pruneSizeHelper(node->NE, tol) +
           pruneSizeHelper(node->SW, tol) + pruneSizeHelper(node->SE, tol);
}

void quadtree::prune(const int tol) {
    pruneHelper(root, tol);
}

void quadtree::pruneHelper(Node* node, const int tol) {
    cout<<"pruneHelper";
    if (node == nullptr) {
        return;
    }

    if (prunable(node, tol)) {
        clearHelper(node->NW);
        clearHelper(node->NE);
        clearHelper(node->SW);
        clearHelper(node->SE);

        node->NW = nullptr;
        node->NE = nullptr;
        node->SW = nullptr;
        node->SE = nullptr;
    } else {
        pruneHelper(node->NW, tol);
        pruneHelper(node->NE, tol);
        pruneHelper(node->SW, tol);
        pruneHelper(node->SE, tol);
    }
}

void quadtree::clear() {
    clearHelper(root);
    root = nullptr;
}

void quadtree::clearHelper(Node* node) {
    cout<<" clearHelper ";
    if (node == nullptr) {
        return;
    }

    clearHelper(node->NW);
    clearHelper(node->NE);
    clearHelper(node->SW);
    clearHelper(node->SE);

    delete node;
}

void quadtree::copy(const quadtree& orig) {
    edge = orig.edge;
    root = copyHelper(orig.root);
}

quadtree::Node* quadtree::copyHelper(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }

    Node* nNode = new Node(node->upLeft, node->dim, node->avg, node->var);
    nNode->NW = copyHelper(node->NW);
    nNode->NE = copyHelper(node->NE);
    nNode->SW = copyHelper(node->SW);
    nNode->SE = copyHelper(node->SE);

    return nNode;
}

