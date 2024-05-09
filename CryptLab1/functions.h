#ifndef functions_h
#define functions_h

using namespace std;

__int128_t powerMod(__int128_t base, __int128_t power, __int128_t mod) {
    __int128_t result = 1;
    base = base % mod;

    while (power > 0) {
        if (power & 1)
            result = (result * base) % mod;

        base = (base * base) % mod;
        power = power >> 1;
    }
    return result;
}

bool isQuadraticResidue(__int128_t a, __int128_t p) {
    for (__int128_t i = 1; i < p; ++i) {
        if (powerMod(i, 2, p) == a) {
            return true;
        }
    }
    return false;
}


__int128_t legendre(__int128_t a, __int128_t p) {
    if (a < 0){
        a *= -1;
    }
    __int128_t modded = a % p;
    if (p < 3 || modded == 0)
        return 0;

    if (isQuadraticResidue(a, p)) {
        return 1;
    }
    return -1;
}

vector<tuple<__int128_t, __int128_t>> primary_factorization(__int128_t n){
    vector<tuple<__int128_t, __int128_t>> prims;
    __int128_t i = 2;
    
    while (i * i <= n){
        __int128_t counter = 0;
        while (n % i == 0){
            n /= i;
            counter += 1;
        }
        if (counter >0){
            tuple<__int128_t, __int128_t> prime(i, counter);
            prims.push_back(prime);
        }
        i += 1;
    }
    if (size(prims) == 0){
        tuple<__int128_t, __int128_t> prime(n, 1);
        prims.push_back(prime);
    }
    return prims;
}

__int128_t jacobi(__int128_t a, __int128_t n) {
    __int128_t result = 1;
    vector<tuple<__int128_t, __int128_t>> prims = primary_factorization(n);
    
    for (__int128_t i = 0; i < size(prims); i++){
        for (__int128_t j = 0; j < get<1>(prims[i]); j++){
            result *= legendre(a, get<0>(prims[i]));
        }
    }
    
    return result;
}

__int128_t ULucasSeq(__int128_t n, __int128_t p, __int128_t q, __int128_t n_t){
    vector<__int128_t> USeq = {0, 1};
    for (__int128_t i = 2; i <= n; i++){
        __int128_t element = (p * USeq[i-1] - q * USeq[i-2]) % n_t;
        USeq.push_back(element);
    }
    
    return USeq.back();
}

__int128_t VLucasSeq(__int128_t n, __int128_t p, __int128_t q, __int128_t n_t){
    vector<__int128_t> VSeq = {2, p};
    for (__int128_t i = 2; i <= n; i++){
        __int128_t element = (p * VSeq[i-1] - q * VSeq[i-2]) % n_t;
        VSeq.push_back(element);
    }
    
    return VSeq.back();
}

bool StrongLucasPrimTest(__int128_t n, __int128_t p, __int128_t q){
    __int128_t sigma = n + 1;
    
    __int128_t d = sigma;
    __int128_t s = 0;

    while (d % 2 == 0) {
        d /= 2;
        s += 1;
    }
    
    if (ULucasSeq(d, p, q, n) % n== 0){
        return true;
    }
    
    __int128_t r = 1;
    for (__int128_t i = 0; i < s; i++){
        if (VLucasSeq(d*r, p, q, n) % n == 0){
            return true;
        }
        r *= 2;
    }
    return false;
}

#endif /* functions_h */
