#pragma once
#include <Core/Core.hpp>
#include <Core/Types.hpp>
#include <Utility/Random.hpp>

namespace Enigma
{
	// multiply string
	String operator*(const String& str, const std::size_t mult)
	{
		String out(str.size() * mult, '\000');
		std::size_t j{ 0 };
		for (std::size_t i{ 0 }; i < mult; ++i)
			for (const auto& c : str)
				out[j++] = c;
		return out;
	}
	String operator*(const std::string_view& str, const std::size_t mult)
	{
		return operator*(String(str), mult);
	}

	static constexpr const std::string_view LOREM_IPSUM = R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit.
Nunc augue nibh, lacinia sit amet condimentum nec, imperdiet non nisi. Nunc maximus metus
quis ipsum fermentum, tempus suscipit sem iaculis. In sit amet odio egestas
orci porttitor ullamcorper. Vestibulum erat risus, pretium sed lacus sed,
finibus ultrices urna. Maecenas fermentum neque viverra diam congue accumsan.
Vivamus id 64 vestibulum justo, et feugiat augue. Donec id malesuada ipsum,
eu malesuada erat. Lorem ipsum dolor sit amet, consectetur adipiscing elit.
Quisque vehicula id enim ut vulputate. Sed vitae nisl ac orci vehicula porttitor
vitae sit amet nunc.)";


#if 0
#pragma region Populate Database With Random Data For Testing
	auto e = std::make_unique<Encryption>();
	for (int i = 1; i <= 1000; i++)
	{
		e->title = Random::Str(Random::Int(3, 255));
		e->cipher.data = std::move(Random::Str(Random::Int(256, 1024 * 10)));
		e->size = e->cipher.data.size();

		Database::AddEncryption(e);
		//if (Database::AddEncryption(e) >= 0)
		//	std::printf("Added Encryption #%d\n", i);
		//else
		//	std::printf("Failed to add Encryptionn #%d\n", i);

		e->title.clear();
		e->date_time.clear();
		e->cipher.data.clear();
	}
	std::cout << "Database total changes since connection: " << Database::GetInstance()->getTotalChanges() << std::endl;
	std::cout << "Database headerStr: " << Database::GetInstance()->getHeaderInfo().headerStr << std::endl;
#pragma endregion
#endif




}
