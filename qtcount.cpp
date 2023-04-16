#include "qtcount.h"

bool qtcount::prunable(Node* node, const int tol) const {

    vector<Node*> nodes;
    RGBAPixel node_avg = node->avg;
    nodes.push_back(node);

    while (!nodes.empty()) {
        Node* curr = nodes.back();
        nodes.pop_back();
        
        Node* NW = curr->NW;
        Node* NE = curr->NE;
        Node* SE = curr->SE;
        Node* SW = curr->SW;

        // is leaf
        if (NW == nullptr && NE == nullptr && SE == nullptr && SW == nullptr) {
            RGBAPixel curr_avg = curr->avg;
            int diff = pow(curr_avg.r - node_avg.r, 2) + pow(curr_avg.g - node_avg.g, 2) + pow(curr_avg.b - node_avg.b, 2);
            if (diff >= tol) return false;
        } else {
            nodes.push_back(NW);
            nodes.push_back(NE);
            nodes.push_back(SE);
            nodes.push_back(SW);
        }
        
    }
    return true;
}