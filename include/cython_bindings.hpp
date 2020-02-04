#pragma once
#include <vector>
#include "../include/BaseSieve.hpp"
using namespace std;

// Convert a vector of gints to a flattened array, then return pointer and size.
pair<int32_t *, uint64_t> gintVectorToArray(vector<gint>);

// Return containers which get slowly copied into python structures.
vector<pair<int32_t, int32_t>> gPrimesToNorm(uint64_t);
vector<pair<int32_t, int32_t>> gPrimesInSector(uint64_t, double, double);
vector<pair<int32_t, int32_t>> gPrimesInBlock(uint32_t, uint32_t, uint32_t, uint32_t);

// Return counts only.
uint64_t gPrimesToNormCount(uint64_t);
uint64_t gPrimesInSectorCount(uint64_t, double, double);
uint64_t gPrimesInBlockCount(uint32_t, uint32_t, uint32_t, uint32_t);

// Return pointer that can be shared with numpy to build np.arrays.
// The first element of the pair holds a pointer to an array of uint32_t ints,
// and the second element of the pair holds the size of the array.
// Real parts and imaginary parts are concatenated together into the array;
// they can be untangled in numpy as needed.
pair<int32_t *, uint64_t> gPrimesToNormAsArray(uint64_t);
pair<int32_t *, uint64_t> gPrimesInSectorAsArray(uint64_t, double, double);
pair<int32_t *, uint64_t> gPrimesInBlockAsArray(uint32_t, uint32_t, uint32_t, uint32_t);

// Histogram of angles of primes to norm.
vector<uint64_t> angularDistribution(uint64_t, uint32_t);

// Gather sector race data and store within class.
class SectorRace {
private:
    uint64_t x, nBins;
    vector<gint> firstSector, secondSector;
    vector<int32_t> normData;
public:
    SectorRace(uint64_t, uint64_t, long double, long double, long double, long double);
    void setNormData();
    pair<int32_t *, uint64_t> getFirstSector();
    pair<int32_t *, uint64_t> getSecondSector();
    int32_t * getNormData();
};

// Gather data for Gaussian moat problem.
class OctantMoat {
private:
    uint64_t x;
    double jumpSize;
    vector<vector<bool>> sieveArray;
    vector<gint> neighbors, toExplore, explored;

public:
    OctantMoat(uint64_t, double);
    void setSieveArray();
    void setNeighbors();
    void setToExplore();
    void explore();
    pair<int32_t *, uint64_t> getExplored();
    pair<int32_t *, uint64_t> getUnexplored();
};