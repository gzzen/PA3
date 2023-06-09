
#include "stats.h"
#include <iostream>

stats::stats(PNG& im) : 
    sumRed(im.width(), vector<long>(im.height(), 0)), 
    sumGreen(im.width(), vector<long>(im.height(), 0)), 
    sumBlue(im.width(), vector<long>(im.height(), 0)), 
    sumsqRed(im.width(), vector<long>(im.height(), 0)), 
    sumsqGreen(im.width(), vector<long>(im.height(), 0)), 
    sumsqBlue(im.width(), vector<long>(im.height(), 0)) {

    for (unsigned int x=0; x<im.width(); x++) {
        for (unsigned int y=0; y<im.height(); y++) {
            RGBAPixel* pixel = im.getPixel(x,y);
            if (x==0 && y==0) {
                // cout<<"哈";
                sumRed[x][y] = pixel->r;
                sumGreen[x][y] = pixel->g;
                sumBlue[x][y] = pixel->b;
                sumsqRed[x][y] = pow(pixel->r, 2);
                sumsqGreen[x][y] = pow(pixel->g, 2);
                sumsqBlue[x][y] = pow(pixel->b, 2);
            } else if (y==0) {
                sumRed[x][y] = pixel->r + sumRed[x-1][y];
                sumGreen[x][y] = pixel->g + sumGreen[x-1][y];
                sumBlue[x][y] = pixel->b + sumBlue[x-1][y];
                sumsqRed[x][y] = pow(pixel->r, 2) + sumsqRed[x-1][y];
                sumsqGreen[x][y] = pow(pixel->g, 2) + sumsqGreen[x-1][y];
                sumsqBlue[x][y] = pow(pixel->b, 2) + sumsqBlue[x-1][y];
            } else if (x==0) {
                sumRed[x][y] = pixel->r + sumRed[x][y-1];
                sumGreen[x][y] = pixel->g + sumGreen[x][y-1];
                sumBlue[x][y] = pixel->b + sumBlue[x][y-1];
                sumsqRed[x][y] = pow(pixel->r, 2) + sumsqRed[x][y-1];
                sumsqGreen[x][y] = pow(pixel->g, 2) + sumsqGreen[x][y-1];
                sumsqBlue[x][y] = pow(pixel->b, 2) + sumsqBlue[x][y-1];
            } else {
                sumRed[x][y] = pixel->r + sumRed[x-1][y] + sumRed[x][y-1] - sumRed[x-1][y-1];
                sumGreen[x][y] = pixel->g + sumGreen[x-1][y] + sumGreen[x][y-1] - sumGreen[x-1][y-1];
                sumBlue[x][y] = pixel->b + sumBlue[x-1][y] + sumBlue[x][y-1] - sumBlue[x-1][y-1];
                sumsqRed[x][y] = pow(pixel->r, 2) + sumsqRed[x-1][y] + sumsqRed[x][y-1] - sumsqRed[x-1][y-1];
                sumsqGreen[x][y] = pow(pixel->g, 2) + sumsqGreen[x-1][y] + sumsqGreen[x][y-1] - sumsqGreen[x-1][y-1];
                sumsqBlue[x][y] = pow(pixel->b, 2) + sumsqBlue[x-1][y] + sumsqBlue[x][y-1] - sumsqBlue[x-1][y-1];
            }
        }
    }
}

long stats::getSum(char channel, pair<int,int> ul, int dim) {
    // x-vec <y-vec <...>> (by columns in matrix)   
    int side = pow(2, dim)-1;
    long x_space_sum = 0;
    long y_space_sum = 0;
    long sum_overlap = 0;
    pair<int,int> end = {ul.first+side, ul.second+side};
    vector<vector<long>> sumv;
    switch (channel) {
        case 'r':
            sumv = sumRed;
            break;
        case 'g':
            sumv = sumGreen;
            break;
        case 'b':
            sumv = sumBlue;
            break;
    }
    if (ul.first != 0) x_space_sum = sumv[ul.first-1][end.second];
    if (ul.second != 0) y_space_sum = sumv[end.first][ul.second-1];
    if (ul.first != 0 && ul.second != 0) sum_overlap = sumv[ul.first-1][ul.second-1];
    
    return sumv[end.first][end.second] - x_space_sum - y_space_sum + sum_overlap;
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {
    int side = pow(2, dim)-1;
    long x_space_sumsq = 0;
    long y_space_sumsq = 0;
    long sumsq_overlap = 0;
    pair<int,int> end = {ul.first+side, ul.second+side};
    vector<vector<long>> sumsqv;
    switch (channel) {
        case 'r':
            sumsqv = sumsqRed;
            break;
        case 'g':
            sumsqv = sumsqGreen;
            break;
        case 'b':
            sumsqv = sumsqBlue;
            break;
    }
    if (ul.first != 0) x_space_sumsq = sumsqv[ul.first-1][end.second];
    if (ul.second != 0) y_space_sumsq = sumsqv[end.first][ul.second-1];
    if (ul.first != 0 && ul.second != 0) sumsq_overlap = sumsqv[ul.first-1][ul.second-1];
    
    return sumsqv[end.first][end.second] - x_space_sumsq - y_space_sumsq + sumsq_overlap;
}

long stats::rectArea(int dim) {
    return pow(pow(2, dim), 2);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {
    double area = (double) rectArea(dim);

    double sum_r = (double) getSum('r', ul, dim);
    double sum_g = (double) getSum('g', ul, dim);
    double sum_b = (double) getSum('b', ul, dim);

    double sumSq_r = (double) getSumSq('r', ul, dim);
    double sumSq_g = (double) getSumSq('g', ul, dim);
    double sumSq_b = (double) getSumSq('b', ul, dim);

    return (sumSq_r - (pow(sum_r, 2) / area)) + (sumSq_g - (pow(sum_g, 2) / area)) + (sumSq_b - (pow(sum_b, 2) / area));

}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    long area = rectArea(dim);
    long avg_r = getSum('r', ul, dim) / area;
    long avg_g = getSum('g', ul, dim) / area;
    long avg_b = getSum('b', ul, dim) / area;
    RGBAPixel* pixel = new RGBAPixel();
    pixel->r = avg_r;
    pixel->g = avg_g;
    pixel->b = avg_b;
    return *pixel;
}
