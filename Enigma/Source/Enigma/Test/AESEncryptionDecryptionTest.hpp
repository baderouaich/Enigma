#pragma once
#include <Enigma.hpp>
#include "Enigma/Algorithm/AES/AESEncryption.hpp"
#include "Enigma/Algorithm/AES/AESDecryption.hpp"

using namespace Enigma;
using namespace std;

class AESEncryptionDecryptionTest
{
public:
    AESEncryptionDecryptionTest()
	{
        aes_encyptor = MakeUnique<Enigma::AESEncryption>();
        aes_decryptor = MakeUnique<Enigma::AESDecryption>();


       // EncryptDecryptText();
        EncryptDecryptFile();
	}

    void EncryptDecryptText()
    {
        String buffer, password;
        cout << "Enter buffer to encrypt: ";
        getline(cin, buffer);
        cout << "Enter password (encryption key): ";
        getline(cin, password);


        String encrypted = aes_encyptor->Encrypt(password, buffer); // iv + cipher
        cout << "Encrypted: " << encrypted << endl;

        String decrypted = aes_decryptor->Decrypt(password, encrypted);;
        cout << "Decrypted: " << decrypted << endl;

    }

    void EncryptDecryptFile()
    {
        
    }



	~AESEncryptionDecryptionTest()
	{
	}

private:
    UniquePtr<AESEncryption> aes_encyptor;
    UniquePtr<AESDecryption> aes_decryptor;

};
