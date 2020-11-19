/*
*	Enigma Entry Point
*/
#pragma once
#ifndef ENIGMA_MAIN_H
#define ENIGMA_MAIN_H

#include <Core/Core.hpp>
#include <Application/Application.hpp>
#include <Logger/Logger.hpp>


#define TESTING false
#include <Test/AESEncryptionDecryptionTest.hpp>


extern Enigma::UniquePtr<Enigma::Application> Enigma::CreateApplication();

int main(int argc, char* argv[])
{
	UNUSED(argc); UNUSED(argv);

	// Initialize Enigma Logger
	Enigma::Logger::Initialize();

#if TESTING
	Enigma::UniquePtr<AESEncryptionDecryptionTest> a(new AESEncryptionDecryptionTest);
#else
	// Create Application
	Enigma::UniquePtr<Enigma::Application> _App = Enigma::CreateApplication();
	// Run Application
	_App->Run();
#endif

	return EXIT_SUCCESS;
}


/*
*	Windows Entry Point (for Release & Distribution)
*/
#if defined(ENIGMA_PLATFORM_WINDOWS)

int WINAPI WinMain(
	_In_ HINSTANCE instance, 
	_In_opt_ HINSTANCE prev_instance, 
	_In_ LPSTR cmd_line, 
	_In_ int cmd_show) 
{
	UNUSED(instance),
	UNUSED(prev_instance),
	UNUSED(cmd_line),
	UNUSED(cmd_show);
	return main(__argc, __argv);
}

#endif

#endif // !ENIGMA_MAIN_H