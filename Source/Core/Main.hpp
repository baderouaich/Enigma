/*
*	Enigma Entry Point
*/
#pragma once
#ifndef ENIGMA_MAIN_H
#define ENIGMA_MAIN_H

#if ENIGMA_TEST
	#include <Tests/Utils/Base64Tests.hpp>
	//#include <Tests/Algorithm/AESTests.hpp>
	//#include <Tests/Algorithm/ChaChaTests.hpp>
	//#include <Tests/Algorithm/TripleDESTests.hpp>
#else
	#include <Enigma.hpp>
#endif

int main(int argc, char* argv[])
{
	// Initialize Enigma Logger
	Enigma::Logger::Initialize();
#if !ENIGMA_TEST
	// Command Line Interface Entry
	if (argc > 1)
	{
		std::unique_ptr<Enigma::CLI> _Cli = std::make_unique<Enigma::CLI>(argc, argv);
		return _Cli->Run();
	}
	// Application Entry
	else
	{
		// Create Enigma UI Application
		std::unique_ptr<Enigma::Application> _App = Enigma::CreateApplication();
		// Run Application
		_App->Run();
		// Exit
		return EXIT_SUCCESS;
	}
#else
	// Run Tests
	return Catch::Session().run(argc, argv);
#endif

}



/*
*	Windows Entry Point (for Release & Distribution)
*/
#if defined(ENIGMA_PLATFORM_WINDOWS) && (defined(ENIGMA_RELEASE) || defined(ENIGMA_DIST))

int WINAPI WinMain(
	_In_ HINSTANCE instance, 
	_In_opt_ HINSTANCE prev_instance, 
	_In_ LPSTR cmd_line, 
	_In_ int cmd_show) 
{
	return main(__argc, __argv);
}

#endif

#endif // !ENIGMA_MAIN_H
