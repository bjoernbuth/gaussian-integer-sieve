#pragma once
#include <vector>
#include "../include/BaseSieve.hpp"
#include "../include/BlockSieve.hpp"
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


// Derived from BlockSieve
class BlockMoat : public BlockSieve {
private:
    // Static variables.
    static bool verbose;
    static double jumpSize;
    static int32_t dx, dy;
    static uint64_t sievingPrimesNormBound;
    static vector<gint> sievingPrimes, nearestNeighbors;

    // Instance variables.
    int32_t x, y;
    int32_t upperWallYPunch;
    uint64_t countVisited;
    int32_t farthestRight;

public:
    // Call this static setter method before any instances of this class are created.
    static void setStatics(int32_t, double, bool = true);

    BlockMoat(int32_t, int32_t);
    void callSieve();
    bool exploreAtGint(int32_t, int32_t, bool = false);
    bool exploreLeftWall();
    void exploreUpperWall();
    pair<int32_t, int32_t> getNextBlock();
};

// Function to handle instances of BlockMoat.
// TODO: Should this instead be a static method of BlockMoat?
void verticalMoat(int32_t, double, bool = true);




// Also derived from BlockSieve
class SegmentedMoat : public BlockSieve {
private:
    // Static variables.
    static bool verbose;
    static double jumpSize;
    static uint32_t previousdy;
    static uint64_t blockSize;  // the number of entries in the block of size dx by dy.
    static uint64_t sievingPrimesNormBound;
    static vector<gint> sievingPrimes, nearestNeighbors;

    // The index of the outer vector determines which component number the inner
    // vector corresponds with.
    static vector<vector<gint>> leftBoundary;

    // Holding counts of component sizes. Individual components are indexed by
    // unsigned longs, the index of this vector.
    static vector<uint64_t> componentSizes;

    // Instance variables.
    int32_t x, dx, dy;
    vector<vector<gint>> rightBoundary;
    // Holds status of components. If component has not propagated to right
    // boundary or merged with another component, it can be forgotten.
    vector<bool> hasComponentPropagated;
    vector<vector<uint32_t>> leftComponentLookUp;

    //debug
    static vector<gint> checker;
    static vector<int> checker2;

public:
    static void setStatics(double, bool = true);
    static void setSievingPrimes();
    static uint64_t countComponent();

    SegmentedMoat(int32_t, int32_t, int32_t);
    void callSieve();
    void exploreComponent(uint32_t, bool = true);
    void exploreLeftBoundary();
    void exploreRightBoundary();
    void runSegment();
    bool hasMainComponentPropagated();
};

uint64_t getCount();