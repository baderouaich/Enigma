#pragma once
#include <gtest/gtest.h>
#include "Algorithm/AES/AESEncryption.hpp"
#include "Algorithm/AES/AESDecryption.hpp"
using namespace std;
using namespace Enigma;

TEST(Encrypt_And_Decrypt_Test, AES)
{
    UniquePtr<AESEncryption> aes_encyptor = MakeUnique<Enigma::AESEncryption>();
    UniquePtr<AESDecryption> aes_decryptor = MakeUnique<Enigma::AESDecryption>();

    String buffer, password;
    cout << "Enter buffer to encrypt: ";
    getline(cin, buffer);
    cout << "Enter password (encryption key): ";
    getline(cin, password);


    String encrypted = aes_encyptor->Encrypt(password, buffer); // iv + cipher
    cout << "Encrypted: " << encrypted << endl;

    String decrypted = aes_decryptor->Decrypt(password, encrypted);;
    cout << "Decrypted: " << decrypted << endl;

    EXPECT_EQ(buffer, decrypted);
}
