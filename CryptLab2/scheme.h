#ifndef scheme_h
#define scheme_h
#include "functions.h"
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Sha1{
    uint32_t digest[5];
    vector<uint8_t> padded_message;
    uint64_t padded_message_size;
    
    void preprocess(string message){
        vector<uint8_t> vec(message.begin(), message.end());
        uint64_t input_size = message.size();
        input_size = message.size();
        
        if (input_size % 64 < 56){
            padded_message_size = input_size + 64 - (input_size % 64);
        }
        else{
            padded_message_size = input_size + 128 - (input_size % 64);
        }
        
        padded_message = vector<uint8_t>(padded_message_size);
        copy(vec.begin(), vec.end(), padded_message.begin());
        
        padded_message[input_size] = 1 << 7;
        for (uint64_t i = input_size; i % 64 != 56; i++) {
            if (i == input_size) {
                continue;
            }
            padded_message[i] = 0;
        }
    
        uint64_t input_bitsize = input_size * 8;
        for (uint8_t i = 0; i < 8; i++) {
            padded_message[padded_message_size - 8 + i] = (input_bitsize >> (56 - 8 * i)) & 0xFF;
        }
    }
    
    uint32_t rolV(uint32_t value, uint32_t bits)
    {
        return (value << bits) | (value >> (32 - bits));
    }
    
    void transform(uint32_t block[80]){
        uint32_t a = digest[0];
        uint32_t b = digest[1];
        uint32_t c = digest[2];
        uint32_t d = digest[3];
        uint32_t e = digest[4];
        uint32_t f;
        uint32_t k;
        
        for (uint64_t i = 0; i < 80; i++){
            if (i >=0 && i <=19){
                f = ((b&c)|((!b)&d));
                k = 0x5A827999;
            }
            else if (i >=20 && i <=39){
                f = b^c^d;
                k = 0x6ED9EBA1;
            }
            else if (i >=40 && i <=59){
                f = (b&c)|(b&d)|(c&d);
                k = 0x8F1BBCDC;
            }
            else{
                f = b^c^d;
                k = 0xCA62C1D6;
            }
            
            uint32_t temp = rolV(a, 5) + f + e + k + block[i];
            e = d;
            d = c;
            c = rolV(b, 30);
            b = a;
            a = temp;
        }
        
        digest[0] += a;
        digest[1] += b;
        digest[2] += c;
        digest[3] += d;
        digest[4] += e;
    }
public:
    Sha1(){
        digest[0] = 0x67452301;
        digest[1] = 0xefcdab89;
        digest[2] = 0x98badcfe;
        digest[3] = 0x10325476;
        digest[4] = 0xc3d2e1f0;
        
        padded_message = {};
        padded_message_size = 0;
    }
    
    string sign(string message){
        preprocess(message);
        uint32_t blocks[80]{};
        
        for (int64_t chunk = 0; chunk * 64 < padded_message_size; chunk++) {
            for (int8_t bid = 0; bid < 16; bid++) {
                blocks[bid] = 0;
                for (int cid = 0; cid < 16; cid++) {
                    if (chunk * 64 + bid * 4 + cid >= 64){
                        blocks[bid] = (blocks[bid] << 8);
                    }
                    else{
                        blocks[bid] = (blocks[bid] << 8) + padded_message[chunk * 64 + bid * 4 + cid];
                    }
                }

                for (uint8_t i = 16; i < 80; i++) {
                    blocks[i] = rolV(blocks[i - 3] ^ blocks[i - 8] ^ blocks[i - 14] ^ blocks[i - 16],1);
                }
            }
            transform(blocks);
        }
        
        ostringstream result;
        for (size_t i = 0; i < sizeof(digest) / sizeof(digest[0]); i++){
            result << hex << (digest[i] >> 4);
            //sprintf(result, "%X", (digest[i] >> 4));
        }
        return result.str();
        }
};

class RSA{
    __int128_t p;
    __int128_t q;
    tuple<__int128_t, __int128_t> publicKey;
    tuple<__int128_t, __int128_t> privateKey;
public:
    void generateKeys(__int128_t bitNums){
        vector<__int128_t> primes = generateBitPrimes(bitNums, 2);
        p = primes[0]; q = primes[1];
        __int128_t n = p * q;
        
        __int128_t phi_n = Carmichael_function(p-1,q-1);
        __int128_t e = generate_coprime(phi_n);
        __int128_t d = extended_euclid(e, phi_n);

        if ((e == 0) || (phi_n == 0)){
            generateKeys(bitNums);
        }
        else{
            this->publicKey = tuple<__int128_t, __int128_t> (e, n);
            this->privateKey = tuple<__int128_t, __int128_t> (d, n);
        }
    }
    
    tuple<vector<__int128_t>, string> encode_message(string message, vector<__int128_t> m, tuple<__int128_t, __int128_t> key){
        Sha1 sha;
        for (int i = 0; i < size(m); i++){
            m[i] = powerMod((int)m[i], get<0>(key), get<1>(key));
        }
        cout << endl << "Encrypted message:" << endl;
        printVector(m);
        string result = sha.sign(message);
        tuple<vector<__int128_t>, string> pair (m, result);
        return pair;
    }
    
    vector<__int128_t> decode_message(vector<__int128_t> c, tuple<__int128_t, __int128_t> key){
        for (int i = 0; i < size(c); i++){
            c[i] = chineseRemainder((int)c[i], get<0>(key), p, q);
        }
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
    
    tuple<__int128_t, __int128_t> generateKeys(__int128_t bitsNum){
        this->rsa = RSA();
        rsa.generateKeys(bitsNum);
        this->privateKey = rsa.getPrivateKey();
        return rsa.getPublicKey();
    }
    
    tuple<vector<__int128_t>, string> sendMessage(string m){
        vector<__int128_t> numericMessage = getNumMessage(m);
        tuple<vector<__int128_t>, string> enc_sig = rsa.encode_message(m, numericMessage, publicKey);
        return enc_sig;
    }
    
    string recieveMessage(tuple<vector<__int128_t>, string> encodedMessage){
        Sha1 sha;
        vector<__int128_t> numericMessage = rsa.decode_message(get<0>(encodedMessage), privateKey);
        string message = getStrMessage(numericMessage);
        string result = sha.sign(message);
        if (result.compare(get<1>(encodedMessage))){
            return "Invalid signature";
        }
        return message;
    }
};

#endif /* scheme_h */
