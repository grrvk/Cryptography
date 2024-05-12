//
//  main.cpp
#include <iostream>
#include "scheme.h"

int main(int argc, const char * argv[]) {
    Person bob = Person("Bob");
    Person alice = Person("Alice");
    string message = "i don't know how to read";
    cout << "Bob is sending: " << message << endl;
    
    //Bob is sending a message to Alice
    tuple<__int128_t, __int128_t> publicKey = alice.generateKeys();
    
    bob.setPublicKey(publicKey);
    vector<__int128_t> encodedMessage = bob.sendMessage(message);
    
    //Alice is recieving a message
    string decodedMessage = alice.recieveMessage(encodedMessage);
    cout << "Decrypted message: "<< decodedMessage << endl;
}
