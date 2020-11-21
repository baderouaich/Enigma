#pragma once
#include <gtest/gtest.h>
#include "Utility/Base64.hpp"
using namespace std;
using namespace Enigma;

TEST(Encode_And_Decode, Base64)
{
    String buffer = R"(
Lorem Ipsum is simply dummy text of the printing and typesetting industry.
 Lorem Ipsum has been the industry's standard dummy text ever since the 1500s,
 when an unknown printer took a galley of type and scrambled it to make a type specimen book.
 It has survived not only five centuries, but also the leap into electronic typesetting,
 remaining essentially unchanged. It was popularised in the 1960s with the release
 of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop
 publishing software like Aldus PageMaker including versions of Lorem Ipsum.
)";

    cout << "Buffer: " << buffer << endl;
    String base64_encoded = Base64::Encode(buffer);
    cout << "Base64 encoded: " << base64_encoded << endl;
    String base64_decoded = Base64::Decode(base64_encoded);
    cout << "Base64 decoded: " << base64_decoded << endl;

    EXPECT_NE(buffer, base64_encoded);
    EXPECT_EQ(buffer, base64_decoded);
}