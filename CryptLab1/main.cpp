#include <iostream>
#include <random>
#include "functions.h"

using namespace std;


bool millerRabin(__int128_t n, __int128_t k, bool setBase = false) {
    if (n % 2 == 0 || n == 1)
        return false;

    __int128_t d = n-1;
    __int128_t s = 0;

    while (d % 2 == 0) {
        d /= 2;
        s += 1;
    }

    __int128_t a, x;
    for (__int128_t i = 1; i <= k; ++i) {
        if (setBase){
            a = 2;
        }
        else{
            a = rand() % (n - 3) + 2;
        }
        x = powerMod(a, d, n);
        
        __int128_t y = 1;
        for (__int128_t j = 0; j < s; j++){
            y = powerMod(x, 2, n);
            if (y == 1 && x != 1 && x != n-1){
                return false;
            }
            x = y;
        }
        
        if (y != 1){
            return false;
        }
    }

    return true;
}


bool bailliePSW(int64_t n) {
    if (!millerRabin(n, 3, true)) return false;
    
    __int128_t d = 5;
    
    __int128_t counter = 0;
    while ((jacobi(d, n) != -1) && (counter<100)) {
        if (d > 0)
            d = -(d + 2);
        else
            d = -(d - 2);
        counter+=1;
    }
    
    __int128_t p = 1;
    __int128_t q = (1 - d) / 4;
    
    if (StrongLucasPrimTest(n, p, q)){
        return true;
    }
    return false;
}

vector<__int128_t> generateBitPrimes(__int128_t bit_length, __int128_t num){
    vector<__int128_t> prims;
    __int128_t n = 2;
    for (__int128_t i = 1; i < bit_length; i++){
        n *= 2;
    }
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<__int128_t> distrib(2, n-1);

    
    __int128_t i = 0;
    while(i < num){
        __int128_t primary = distrib(gen);
        if (bailliePSW(primary) && (find(prims.begin(), prims.end(), primary) == prims.end())){
            prims.push_back(primary);
            i += 1;
        }
    }

    return prims;
}

int main(int argc, const char * argv[]) {
    __int128_t generatedPrime = generateBitPrimes(10, 1)[0];
    cout << "Generated prime " << (int64_t)generatedPrime << endl;
    diffOutput(generatedPrime);
    
    int64_t result = powerMod(12, 15, 14);
    cout << "12^15 mod 14 = " << result << endl;
    
    cout << "Number 1523 is prime - " << (millerRabin(1523, 3) ? "true" : "false") << endl;
    
    cout << "Number 563 is prime - " << (bailliePSW(563) ? "true" : "false") << endl;

    return 0;
}
