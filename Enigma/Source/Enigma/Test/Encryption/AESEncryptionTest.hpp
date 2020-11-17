#pragma once
#include <Enigma.hpp>
#include "Enigma/Encryption/AESEncryption.hpp"
using namespace Enigma;

class AESEncryptionTest
{
public:
	AESEncryptionTest()
	{
        
        String buffer;
        using namespace std;
        cout << "Enter buffer to encrypt: ";
        getline(cin, buffer);

        UniquePtr<AESEncryption> aes(new AESEncryption());
        String encrypted_buffer = aes->Encrypt(buffer);
        cout << "Encrypted buffer/cipher: " << encrypted_buffer << endl;
        cout << "IV: " << aes->GetIV() << endl;
        cout << "Key: " << aes->GetKey() << endl;
        //String decrypted_buffer = aes->Decrypt(encrypted_buffer);
        //cout << "Decrypted buffer/cipher: " << buffer << endl;
        //cout << "IV: " << aes.GetIV() << endl;
	}

	~AESEncryptionTest()
	{
	}

private:

};
