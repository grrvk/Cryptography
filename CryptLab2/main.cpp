//
//  main.cpp
#include <iostream>
#include <chrono>
#include "scheme.h"

int main(int argc, const char * argv[]) {
    Person bob = Person("Bob");
    Person alice = Person("Alice");
    string message = "i dont know how to read";

    cout << "Bob is sending: " << message << endl;
    //Bob is sending a message to Alice
    tuple<__int128_t, __int128_t> publicKey = alice.generateKeys(16);
    
    bob.setPublicKey(publicKey);
    tuple<vector<__int128_t>, string> encodedMessage = bob.sendMessage(message);
    
    cout << "Signature: " << get<1>(encodedMessage) << endl << endl;
    
    //get<0>(encodedMessage).push_back(23);
    
    //Alice is recieving a message
    string decodedMessage = alice.recieveMessage(encodedMessage);
    auto end2 = chrono::high_resolution_clock::now();
    cout << "Decrypted message: "<< decodedMessage << endl;
}
