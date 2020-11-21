#pragma once
#include <gtest/gtest.h>
#include "Algorithm/AES/AESEncryption.hpp"
#include "Algorithm/AES/AESDecryption.hpp"
using namespace std;
using namespace Enigma;


TEST(Encrypt_And_Decrypt_Text, AES)
{
    UniquePtr<AESEncryption> aes_encyptor = MakeUnique<Enigma::AESEncryption>();
    UniquePtr<AESDecryption> aes_decryptor = MakeUnique<Enigma::AESDecryption>();
    String buffer, password;
    String encrypted, decrypted;
    
    cout << "\nEnter buffer to encrypt: ";
    getline(cin, buffer);
    cout << "\nEnter password (encryption key): ";
    getline(cin, password);


    encrypted = aes_encyptor->Encrypt(password, buffer); // iv + cipher
    cout << "\nEncrypted: " << encrypted;

    decrypted = aes_decryptor->Decrypt(password, encrypted);;
    cout << "\nDecrypted: " << decrypted << endl;

    EXPECT_NE(buffer, encrypted);
    EXPECT_EQ(buffer, decrypted);
}

