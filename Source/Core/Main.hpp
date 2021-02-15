/*
*	Enigma Entry Point
*/
#pragma once
#ifndef ENIGMA_MAIN_H
#define ENIGMA_MAIN_H

#if ENIGMA_TEST
	//#include <Tests/Algorithm/AESTests.hpp>
	//#include <Tests/Algorithm/ChaChaTests.hpp>
	//#include <Tests/Algorithm/TripleDESTests.hpp>
	//#include <Tests/Algorithm/TwofishTests.hpp>
	//#include <Tests/Algorithm/IDEATests.hpp>
	//#include <Tests/Utils/Base64Tests.hpp>
	//#include <Tests/Utils/GZipTests.hpp>
#else
	#include <Enigma.hpp>
#endif

#if 0
#include "HTTPRequest.hpp"
constexpr const auto URL = "api.github.com/repos/BaderEddineOuaich/Enigma/releases/latest";

try
{
	// you can pass http::InternetProtocol::V6 to Request to make an IPv6 request
	http::Request request(URL);

	// send a get request
	const http::Response response = request.send("GET");
	if (response.status == http::Response::Status::Ok)
	{
		std::cout << std::string(response.body.begin(), response.body.end()) << '\n'; // print the result
	}
	else
	{
		std::string err;
#define CASE(e) case http::Response::Status::e:  err = #e; break;
		switch (response.status)
		{
			CASE(Continue)
				CASE(SwitchingProtocol)
				CASE(Processing)
				CASE(EarlyHints)

				CASE(Ok)
				CASE(Created)
				CASE(Accepted)
				CASE(NonAuthoritativeInformation)
				CASE(NoContent)
				CASE(ResetContent)
				CASE(PartialContent)
				CASE(MultiStatus)
				CASE(AlreadyReported)
				CASE(ImUsed)

				CASE(MultipleChoice)
				CASE(MovedPermanently)
				CASE(Found)
				CASE(SeeOther)
				CASE(NotModified)
				CASE(UseProxy)
				CASE(TemporaryRedirect)
				CASE(PermanentRedirect)

				CASE(BadRequest)
				CASE(Unauthorized)
				CASE(PaymentRequired)
				CASE(Forbidden)
				CASE(NotFound)
				CASE(MethodNotAllowed)
				CASE(NotAcceptable)
				CASE(ProxyAuthenticationRequired)
				CASE(RequestTimeout)
				CASE(Conflict)
				CASE(Gone)
				CASE(LengthRequired)
				CASE(PreconditionFailed)
				CASE(PayloadTooLarge)
				CASE(UriTooLong)
				CASE(UnsupportedMediaType)
				CASE(RangeNotSatisfiable)
				CASE(ExpectationFailed)
				CASE(MisdirectedRequest)
				CASE(UnprocessableEntity)
				CASE(Locked)
				CASE(FailedDependency)
				CASE(TooEarly)
				CASE(UpgradeRequired)
				CASE(PreconditionRequired)
				CASE(TooManyRequests)
				CASE(RequestHeaderFieldsTooLarge)
				CASE(UnavailableForLegalReasons)

				CASE(InternalServerError)
				CASE(NotImplemented)
				CASE(BadGateway)
				CASE(ServiceUnavailable)
				CASE(GatewayTimeout)
				CASE(HttpVersionNotSupported)
				CASE(VariantAlsoNegotiates)
				CASE(InsufficientStorage)
				CASE(LoopDetected)
				CASE(NotExtended)
				CASE(NetworkAuthenticationRequired)
		default: err = "unknown";
		}
		std::cerr << "Error: " << err;
	}
}
catch (const std::exception& e)
{
	std::cerr << "Request failed, error: " << e.what() << '\n';
}
return 0;
#endif

//#pragma comment(lib, "wldap32.lib" )
//#pragma comment(lib, "crypt32.lib" )
//#pragma comment(lib, "Ws2_32.lib")

#ifndef CURL_STATICLIB
	#define CURL_STATICLIB
#endif
#include <curl/curl.h>

constexpr const auto URL = "https://api.github.com/repos/BaderEddineOuaich/Enigma/releases/latest";

int main(int argc, char* argv[])
{
	//TODO use cpr :))
	CURL* curl = curl_easy_init();

	std::cout << curl;
	curl_easy_cleanup(curl);
	return 0;

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
