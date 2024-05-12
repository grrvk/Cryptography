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

int64_t generate_Prim(int64_t n){
    int64_t primary;
    do{
        primary = rand() % n + 2;
    }
    while(!millerRabin(primary, 3));
    return primary;
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


int main(int argc, const char * argv[]) {
    cout << "Generated primary " << generate_Prim(1000) << endl;
    
    int64_t result = powerMod(12, 15, 14);
    cout << "12^15 mod 14 = " << result << endl;
    
    cout << "Number 1523 is prime - " << (millerRabin(1523, 3) ? "true" : "false") << endl;
    
    cout << "Number 563 is prime - " << (bailliePSW(563) ? "true" : "false") << endl;

    return 0;
}
