#include "Main.hpp"
//using namespace std;


//int main(int argc, const char* argv[]){
//    if (argc != 2) {
//        cerr << "Specify the sieve array norm bound." << endl;
//        return 1;
//    }
//    long x = strtol(argv[1], nullptr, 10);  // convert command line input to long


//int main() {
//    long x = pow(10, 9);
//    long y = pow(10, 8);
//    long z = 100;
//    SegmentedSieve s(x, y, z);
//    s.run();
//    s.countBigPrimes();
//    return 0;
//}

//int main() {
//    long x = 10000;
//    DonutSieve s(x);
//    s.run();
//    s.sortBigPrimes();
//    s.printBigPrimes();
//    // s.printSieveArray();
//    // s.countBigPrimes();
//    return 0;
//}
//
// int cythonTest(int x) {
//    QuadrantSieve s(x);
//    vector<gint> P = s.run();
//    return P.size();
//}

int cythonTest2(int x) { return 2 * x + 1; }

