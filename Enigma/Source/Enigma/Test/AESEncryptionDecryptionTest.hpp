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
        
        String buffer, password;
        cout << "Enter buffer to encrypt: ";
        getline(cin, buffer);
        cout << "Enter password (encryption key): ";
        getline(cin, password);

        UniquePtr<AESEncryption> aes_encyptor(new AESEncryption());
        UniquePtr<AESDecryption> aes_decriptor(new AESDecryption());

        String encrypted = aes_encyptor->Encrypt(password, buffer); // iv + cipher
        cout << "Encrypted: " << encrypted << endl;

        String decrypted = aes_decriptor->Decrypt(password, encrypted);;
        cout << "Decrypted: " << decrypted << endl;

	}



	~AESEncryptionDecryptionTest()
	{
	}

private:

};
