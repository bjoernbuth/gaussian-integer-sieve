#include <iostream>
#include <cmath>
#include "gint.hpp"
using namespace std;

class SegmentedSieve {
private:
    long x, y, z;
    long maxNorm;  // need primes with norm up to sqrt(maxNorm)
    vector<vector<bool>> sieveArray;
    vector<Gint> smallPrimes;
    vector<Gint> primes;  // to hold primes after sieving

public:
    SegmentedSieve(long, long, long);
    void initializeSieveArray();
    void printSieveArray();
    void setSmallPrimes();
    void crossOffMultiples(Gint);
    void sieve();

};

SegmentedSieve::SegmentedSieve(long x, long y, long z) {
    this->x = x;  // x-coordinate of lower left-hand corner of segment block
    this->y = y;  // y-coordinate of lower left-hand corner of segment block
    this->z = z;  // side length of segment block
    maxNorm = (x + z) * (y + z);
}

void SegmentedSieve::initializeSieveArray() {
    // sieveArray holds values for Gint's with x <= a <= x + z and y <= b <= y + z
    for (long i = 0; i <= z; i ++) {
        vector<bool> column((unsigned long)(z + 1), true);
        sieveArray.push_back(column);
    }
}

void SegmentedSieve::printSieveArray() {
    // Print sieve array with same orientation as that in the complex plane.
    for (long v = z; v >= 0; v--) {
        string row;
        for (long u = 0; u <= z; u++){
            if (sieveArray[u][v]) {
                row += '*';  // found a prime
            } else {
                row += ' ';  // found a composite
            }
        }
        cout << row << endl;
    }
}

void SegmentedSieve::setSmallPrimes() {
    smallPrimes = readPrimesFromFile(isqrt(maxNorm));
}

void SegmentedSieve::crossOffMultiples(Gint g) {
    // Let a + bi be the Gint and c + di be the co-factor of the multiple we seek.
    // We need the product (a + bi)(c + di) to be contained in the segment block.
    // This gives inequalities x <= ac - bd <= x + z and y <= ad + bc <= y + z.
    // Solve these for c to get (ax + by) / (a^2 + b^2) <= c <= (a(x+z) + b(y+z)) / (a^2 + b^2).
    // Then d is defined by some max and min conditions.

    long cLower, cUpper;
    // Trick: to get ceiling of integer division n / m, call (n + m - 1) / m.
    // Trick only works when m, n > 0.
    if (g.b()) {
        cLower = (g.a() * x + g.b() * y + g.norm() - 1) / g.norm();  // using trick
        cUpper = (g.a() * (x + z) + g.b() * (y + z)) / g.norm();
    } else {
        cLower = (x + g.a() - 1) / g.a();  // using trick
        cUpper = (x + z) / g.a();
    }
    for (long c = cLower; c <= cUpper; c++) {
        long dLower, dUpper;
        if (g.b()) {
            // trick doesn't work because of sign; instead using ceil.
            double d1 = double(g.a() * c - x - z) / double(g.b());
            double d2 = double(y - g.b() * c) / double(g.a());
            dLower = long(ceil(max(d1, d2)));
            d1 = double(g.a() * c - x) / double(g.b());
            d2 = double(y + z - g.b() * c) / double(g.a());
            dUpper = long(floor(min(d1, d2)));
        } else {
            dLower = (y + g.a() - 1) / g.a();  // using trick
            dUpper = (y + z) / g.a();
        }
        long u = g.a() * c - g.b() * dLower - x;  // u = ac - bd - x
        long v = g.b() * c + g.a() * dLower - y;  // v = bc + ad - y
        for (long d = dLower; d <= dUpper; d++) {
            cout << u << "  " << v << "  " << c << "  " << d << endl;
            sieveArray[u][v] = false;
            u -= g.b();
            v += g.a();
        }
    }
}

void SegmentedSieve::sieve() {
    for (Gint p : smallPrimes) {
        cout << p.a() << "  " << p.b() << endl;
        crossOffMultiples(p);
    }
}


int main() {
    long x = 30000;
    long y = 10000;
    long z = 150;
    SegmentedSieve ss(x, y, z);
    ss.initializeSieveArray();
    ss.setSmallPrimes();
    ss.sieve();
    ss.printSieveArray();
}