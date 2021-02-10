#pragma once
#include <Core/Core.hpp>
#include <Core/Types.hpp>

namespace Enigma
{
	// multiple string
	String operator*(const String& str, const size_t& mult)
	{
		String out(str.size() * mult, '\000');
		size_t i{ 0 }, j{ 0 };
		for (i; i < mult; ++i)
			for (const auto& c : str)
				out[j++] = c;
		return out;
	}

	static const String LOREM_IPSUM = R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit.
Nunc augue nibh, lacinia sit amet condimentum nec, imperdiet non nisi. Nunc maximus metus
quis ipsum fermentum, tempus suscipit sem iaculis. In sit amet odio egestas
orci porttitor ullamcorper. Vestibulum erat risus, pretium sed lacus sed,
finibus ultrices urna. Maecenas fermentum neque viverra diam congue accumsan.
Vivamus id 64 vestibulum justo, et feugiat augue. Donec id malesuada ipsum,
eu malesuada erat. Lorem ipsum dolor sit amet, consectetur adipiscing elit.
Quisque vehicula id enim ut vulputate. Sed vitae nisl ac orci vehicula porttitor
vitae sit amet nunc.)";


	String LoremIpsum(const size_t mul)
	{
		return LOREM_IPSUM * mul;
	}

}
