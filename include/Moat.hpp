#pragma once
#include <vector>
#include "../include/BaseSieve.hpp"
using namespace std;


// Gather data for Gaussian moat problem.
class OctantMoat {
private:
    uint64_t normBound;
    double jumpSize;
    bool verbose;
    vector<vector<bool>> sieveArray;
    vector<gint> nearestNeighbors, currentComponent;
    vector<vector<gint>> allComponents;

public:
    OctantMoat(uint64_t, double, bool = true);
    void setNearestNeighbors();
    void exploreComponent(int32_t, int32_t);
    uint32_t getComponentSize();
    gint getComponentMaxElement();
    void exploreAllComponents();
    vector<gint> getCurrentComponent();
    void printCurrentComponent();
    vector<gint> getUnexplored();
    vector<vector<gint>> getAllComponents();
};


class VerticalMoat {
private:
    uint32_t realPart;
    double jumpSize;
    bool verbose;
    uint64_t normBound;
    vector<gint> sievingPrimes;
    uint32_t x;
    uint32_t y;
    uint32_t dx;
    uint32_t dy;

public:
    VerticalMoat(uint32_t, double, bool = true);
    void exploreBlock();
};