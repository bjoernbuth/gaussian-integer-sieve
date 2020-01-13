/* Perform sieving in the box defined by [x, x + dx) x [y, y + dy).
 * Must have x > 0, y >= 0, dx > 0, dy > 0.
 * Using a donut sieve approach; this will be most efficient if all
 * of x, y, dx, and dy are multiples of 10.
 */

#include <iostream>
#include <cmath>
#include "../include/DonutSieve.hpp"
#include "../include/BlockDonutSieve.hpp"
using namespace std;

// Using an initializer list
BlockDonutSieve::BlockDonutSieve(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy, bool verbose)
        : SieveTemplate<uint32_t>((uint64_t)(x + dx - 1) * (uint64_t)(y + dy - 1), verbose)
        , x(x)  // x-coordinate of lower left-hand corner of segment block
        , y(y)  // y-coordinate of lower left-hand corner of segment block
        , dx(dx)  // horizontal side length of segment block
        , dy(dy) // vertical side length of block


        // For a fixed c, d will iterate by d += 2 or d += 4, similar to the classic
        // wheel sieve. Got these iteration patterns in printDonut(), then pasted them
        // here so they can be hard coded in constructor.

        // dStart array determines the starting value of d for a given c during crossOffMultiples()
        , dStart{1, 0, 3, 0, 1, 2, 1, 0, 3, 0}

        // donut array tracks how to move from one d to another during crossOffMultiples()
        // note: rows and columns reversed from usual complex plane orientation -- don't get confused.
        , gapDonut{{0, 2, 0, 4, 0, 0, 0, 2, 0, 2},
                   {4, 0, 0, 0, 2, 0, 4, 0, 0, 0},
                   {0, 0, 0, 2, 0, 2, 0, 6, 0, 0},
                   {2, 0, 6, 0, 0, 0, 0, 0, 2, 0},
                   {0, 4, 0, 0, 0, 4, 0, 0, 0, 2},
                   {0, 0, 2, 0, 2, 0, 2, 0, 4, 0},
                   {0, 4, 0, 0, 0, 4, 0, 0, 0, 2},
                   {2, 0, 6, 0, 0, 0, 0, 0, 2, 0},
                   {0, 0, 0, 2, 0, 2, 0, 6, 0, 0},
                   {4, 0, 0, 0, 2, 0, 4, 0, 0, 0}}

        , bitDonut{{99, 0, 99, 1, 99, 99, 99, 2, 99, 3},
                   {4, 99, 99, 99, 5, 99, 6, 99, 99, 99},
                   {99, 99, 99, 7, 99, 8, 99, 9, 99, 99},
                   {10, 99, 11, 99, 99, 99, 99, 99, 12, 99},
                   {99, 13, 99, 99, 99, 14, 99, 99, 99, 15},
                   {99, 99, 16, 99, 17, 99, 18, 99, 19, 99},
                   {99, 20, 99, 99, 99, 21, 99, 99, 99, 22},
                   {23, 99, 24, 99, 99, 99, 99, 99, 25, 99},
                   {99, 99, 99, 26, 99, 27, 99, 28, 99, 99},
                   {29, 99, 99, 99, 30, 99, 31, 99, 99, 99}}

        , realPartDecompress{0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9}
        , imagPartDecompress{1, 3, 7, 9, 0, 4, 6, 3, 5, 7, 0, 2, 8, 1, 5, 9, 2, 4, 6, 8, 1, 5, 9, 0, 2, 8, 3, 5, 7, 0, 4, 6}
{}

// Calling the fastest sieve method available to generate small primes.
void BlockDonutSieve::setSmallPrimes() {
    if (verbose) {
        cerr << "Calling the DonutSieve to generate smallPrimes..." << endl;
    }
    DonutSieve s((uint64_t)isqrt(maxNorm), false);
    s.run();
    smallPrimes = s.getBigPrimes();
}

// Sieve array holds indices corresponding to gints with x <= a < x + dx and
// y <= b < y + dy. Since we are using a donut approach, the size of the sieve
// array is dx/10 by dy/10.
void BlockDonutSieve::setSieveArray() {
    if (verbose) {
        cerr << "Building donut sieve array..." << endl;
    }
    for (uint32_t i = 0; i < dx / 10; i++) {
        vector<uint32_t> column(dy / 10, (uint32_t)pow(2, 32) - 1);
        sieveArray.push_back(column);
    }
    if (verbose) {
        printSieveArrayInfo();
    }
}
