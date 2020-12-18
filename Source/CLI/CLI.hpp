#pragma once
#ifndef ENIGMA_CLI_H
#define ENIGMA_CLI_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#include <cxxopts.hpp> // cmd parser LIB


NS_ENIGMA_BEGIN

class ENIGMA_API CLI
{
public:
	CLI(i32 argc, char** argv)
	{
		m_args.resize(argc);
		for (i32 i(0); i < argc; ++i)
			m_args[i] = std::move(argv[i]);
	}
	~CLI() noexcept
	{

	}


	i32 Run() 
	{
		ENIGMA_INFO(ENIGMA_CURRENT_FUNCTION);
		for (const auto& arg : m_args)
			ENIGMA_TRACE(arg.c_str());
		return EXIT_SUCCESS;
	}

private:
	std::vector<String> m_args;
};

NS_ENIGMA_END

#endif // !ENIGMA_CLI_H
