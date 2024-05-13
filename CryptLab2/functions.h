#ifndef functions_h
#define functions_h
#include "lab1.h"
#include <random>

using namespace std;

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

__int128_t chineseRemainder(__int128_t c, __int128_t d, __int128_t p, __int128_t q) {
    __int128_t a1 = powerMod(c % q, d % (q-1), q);
    __int128_t a2 = powerMod(c % p, d % (p-1), p);
    __int128_t m = 1;

    while (true)
    {
        if (m%q == a1 and m%p == a2)
            return m;
        m++;
    }
}

__int128_t gcd(__int128_t p, __int128_t q){
    __int128_t gcd = 1;
    
    for (int i = 1; i <=  q; ++i) {
        if (p % i == 0 && q % i ==0) {
          gcd = i;
        }
    }
    
    return gcd;
}

vector<__int128_t> get_all_coprimes(__int128_t n){
    vector<__int128_t> coprimes;
    for (__int128_t i = 1; i < n; i++){
        if (gcd(n, i) == 1){
            coprimes.push_back(i);
        }
    }
    return coprimes;
}

__int128_t generate_coprime(__int128_t n){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<__int128_t> distrib(0, n-1);
    bool cycle = true;
    
    __int128_t i;
    while(cycle){
        i = distrib(gen);
        if (gcd(n, i) == 1){
            break;
        }
    }
    return i;
}

__int128_t Carmichael_function(__int128_t p, __int128_t q){
    return (p / gcd(p, q)) * q;
}

__int128_t extended_euclid(__int128_t a, __int128_t b) {
    __int128_t s = 0; __int128_t old_s = 1;
    __int128_t t = 1; __int128_t old_t = 0;
    __int128_t r = b; __int128_t old_r = a;

    while(r != 0) {
        __int128_t quotient = old_r/r;
        __int128_t temp = r;
        r = old_r - quotient*r;
        old_r = temp;
        temp = s;
        s = old_s - quotient*s;
        old_s = temp;
        temp = t;
        t = old_t - quotient*t;
        old_t = temp;
    }
    if (old_s < 0){
        old_s += b;
    }
    return old_s;
}

vector<__int128_t> getNumMessage(string m) {
    vector<__int128_t> message;
    for (char c : m) {
        message.push_back(static_cast<int>(c));
    }
    return message;
}

string getStrMessage(vector<__int128_t> m) {
    string message;
    for (__int128_t symbol: m) {
        message += static_cast<char>(symbol);
    }
    return message;
}

void printVector(vector<__int128_t> vec){
    for (auto & element : vec) {
        cout << element << " ";
    }
    cout << endl;
}

#endif /* functions_h */
