#ifndef scheme_h
#define scheme_h
#include "functions.h"

using namespace std;


class RSA{
    __int128_t p;
    __int128_t q;
    tuple<__int128_t, __int128_t> publicKey;
    tuple<__int128_t, __int128_t> privateKey;
public:
    void generateKeys(){
        vector<__int128_t> primes = generateBitPrimes(9, 2);
        p = primes[0]; q = primes[1];
        __int128_t n = p * q;
        
        __int128_t phi_n = Carmichael_function(p-1,q-1);
        __int128_t e = generate_coprime(phi_n);
        __int128_t d = extended_euclid(e, phi_n);

        if ((e == 0) || (phi_n == 0)){
            generateKeys();
        }
        else{
            this->publicKey = tuple<__int128_t, __int128_t> (e, n);
            this->privateKey = tuple<__int128_t, __int128_t> (d, n);
        }
    }
    
    vector<__int128_t> encode_message(vector<__int128_t> m, tuple<__int128_t, __int128_t> key){
        for (int i = 0; i < size(m); i++){
            m[i] = powerMod((int)m[i], get<0>(key), get<1>(key));
        }
        cout << endl << "Encrypted message:" << endl;
        printVector(m); cout << endl;
        return m;
    }
    
    vector<__int128_t> decode_message(vector<__int128_t> c, tuple<__int128_t, __int128_t> key){
        for (int i = 0; i < size(c); i++){
            c[i] = chineseRemainder((int)c[i], get<0>(key), p, q);
        }
        //printVector(c);
        return c;
    }
    
    tuple<__int128_t, __int128_t> getPublicKey(){
        return publicKey;
    }
    
    tuple<__int128_t, __int128_t> getPrivateKey(){
        return privateKey;
    }
};

class Person{
    string name;
    RSA rsa;
    tuple<__int128_t, __int128_t> privateKey;
    tuple<__int128_t, __int128_t> publicKey;
public:
    Person(string name){
        this->name = name;
    }
    
    void setPublicKey(tuple<__int128_t, __int128_t> key){
        this->publicKey = key;
    }
    
    tuple<__int128_t, __int128_t> generateKeys(){
        this->rsa = RSA();
        rsa.generateKeys();
        this->privateKey = rsa.getPrivateKey();
        return rsa.getPublicKey();
    }
    
    vector<__int128_t> sendMessage(string m){
        vector<__int128_t> numericMessage = getNumMessage(m);
        //printVector(numericMessage);
        vector<__int128_t> encodedMessage = rsa.encode_message(numericMessage, publicKey);
        return encodedMessage;
    }
    
    string recieveMessage(vector<__int128_t> c){
        vector<__int128_t> numericMessage = rsa.decode_message(c, privateKey);
        string message = getStrMessage(numericMessage);
        return message;
    }
};

#endif /* scheme_h */
