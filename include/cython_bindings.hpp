#pragma once
#include <vector>
#include "../include/BaseSieve.hpp"
using namespace std;

// Returning containers which get slowly copied into python structures.
vector<pair<uint32_t, uint32_t>> gPrimesToNorm(uint64_t);
vector<pair<uint32_t, uint32_t>> gPrimesInSector(uint64_t, double, double);
vector<pair<uint32_t, uint32_t>> gPrimesInBlock(uint32_t, uint32_t, uint32_t, uint32_t);

// Returning counts only.
uint64_t gPrimesToNormCount(uint64_t);
uint64_t gPrimesInSectorCount(uint64_t, double, double);
uint64_t gPrimesInBlockCount(uint32_t, uint32_t, uint32_t, uint32_t);

// Return pointer that can be shared with numpy to build np.arrays.
// The first element of the pair holds a pointer to an array of uint32_t ints,
// and the second element of the pair holds the size of the array.
// Real parts and imaginary parts are concatenated together into the array;
// they can be untangled in numpy as needed.
pair<uint32_t *, uint64_t> gPrimesToNormAsArray(uint64_t);
pair<uint32_t *, uint64_t> gPrimesInSectorAsArray(uint64_t, double, double);
pair<uint32_t *, uint64_t> gPrimesInBlockAsArray(uint32_t, uint32_t, uint32_t, uint32_t);

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
    pair<uint32_t *, uint64_t> getFirstSector();
    pair<uint32_t *, uint64_t> getSecondSector();
    int32_t * getNormData();
};


class Moat {
private:
    double jumpSize;
    vector<gint> neighbors, toExplore, explored;
    vector<vector<bool>> sieveArray;

public:
    Moat(double jumpSize) : jumpSize(jumpSize) {}
    void setSieveArray();
    void setNeighbors();
    void explore();
};