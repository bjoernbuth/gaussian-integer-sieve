#include <iostream>
#include <random>
#include "../include/OctantSieve.hpp"
#include "../include/OctantDonutSieve.hpp"
#include "../include/BlockSieve.hpp"
#include "../include/BlockDonutSieve.hpp"
#include "../include/SectorSieve.hpp"
using namespace std;


int main() {
    cout << "\nTesting and Timing Octant Sieve and Octant Donut Sieve\n" << endl;
    cout << " | norm bound | # of primes including associates | octant sieve time | octant donut sieve time | " << endl;
    cout << " |------------|----------------------------------|-------------------|-------------------------| " << endl;

    for (int j = 20; j <= 25; j++) {
        auto startTime = chrono::high_resolution_clock::now();
        OctantSieve o(pow(2, j), false);
        o.run();
        vector<gint> oP = o.getBigPrimes(false);  // not sorting yet
        auto endTime = chrono::high_resolution_clock::now();
        auto totalTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
        double octantTime = double(totalTime.count()) / 1000.0;

        startTime = chrono::high_resolution_clock::now();
        OctantDonutSieve d(pow(2, j), false);
        d.run();
        vector<gint> dP = d.getBigPrimes(false);  // not sorting yet
        endTime = chrono::high_resolution_clock::now();
        totalTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
        double donutTime = double(totalTime.count()) / 1000.0;

        cout << " | 2^" << j
             << " | " << 4 * oP.size()
             << " | " << octantTime
             << " s | " << donutTime
             << " s | " << endl;

        // Sorting generated primes and checking if two lists are equal.
        sort(oP.begin(), oP.end());
        sort(dP.begin(), dP.end());
        assert(oP == dP);
    }

    cout << "\nTesting and Timing Block Sieve and Block Donut Sieve\n" << endl;
    cout << " | block | # of primes | block sieve time | block donut sieve time | " << endl;
    cout << " |-------|-------------|------------------|------------------------| " << endl;

    default_random_engine generator;
    uniform_int_distribution<int> distInt(1,9);
    for (int j = 1; j <= 5; j++) {
        // Arbitrary testing parameters.
        uint32_t x = distInt(generator) * pow(10, 6);
        uint32_t y = distInt(generator) * pow(10, 6);
        uint32_t dx = distInt(generator) * 100;
        uint32_t dy = distInt(generator) * 100;

        auto startTime = chrono::high_resolution_clock::now();
        BlockSieve b(x, y, dx, dy, false);
        b.run();
        vector<gint> bP = b.getBigPrimes(false);  // not sorting yet
        auto endTime = chrono::high_resolution_clock::now();
        auto totalTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
        double blockTime = double(totalTime.count()) / 1000.0;

        startTime = chrono::high_resolution_clock::now();
        BlockDonutSieve d(x, y, dx, dy, false);
        d.run();
        vector<gint> dP = d.getBigPrimes(false);  // not sorting yet
        endTime = chrono::high_resolution_clock::now();
        totalTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
        double donutTime = double(totalTime.count()) / 1000.0;

        cout << " | [" << x << ", " << x + dx
             << ") x [" << y << ", " << y + dy
             << ") | " << bP.size()
             << " | " << blockTime
             << " s | " << donutTime
             << " s | " << endl;

        // Sorting generated primes and checking if two lists are equal.
        sort(bP.begin(), bP.end());
        sort(dP.begin(), dP.end());
        assert(bP == dP);
    }

    cout << "\nTiming Sector Sieve\n" << endl;
    cout << " | alpha | beta | norm bound | # of primes | time | " << endl;
    cout << " |-------|------|------------|-------------|------| " << endl;
    uniform_real_distribution<double> distReal(0.0, M_PI_4 - 0.01);
    for (int j = 25; j <= 30; j++) {
        uint64_t x = pow(2, j);
        double alpha = distReal(generator);
        double beta = distReal(generator);
        double temp = max(alpha, beta);
        alpha = min(alpha, beta);
        beta = temp;

        auto startTime = chrono::high_resolution_clock::now();
        SectorSieve s(x, alpha, beta, false);
        s.run();
        vector<gint> sP = s.getBigPrimes(false);  // not sorting yet
        auto endTime = chrono::high_resolution_clock::now();
        auto totalTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
        double sectorTime = double(totalTime.count()) / 1000.0;
        cout << " | " << alpha
             << " | " << beta
             << " | 2^" << j
             << " | " << sP.size()
             << " | " << sectorTime
             << " | " << endl;

    }
    return 0;
}