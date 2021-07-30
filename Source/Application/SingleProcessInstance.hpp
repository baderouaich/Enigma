#pragma once
#ifndef ENIGMA_SINGLE_INSTANCE_H
#define ENIGMA_SINGLE_INSTANCE_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#if defined(ENIGMA_PLATFORM_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#if defined(_WIN32_WINNT)
		#undef _WIN32_WINNT
		#define _WIN32_WINNT 0x06000100
	#endif
	#include <SDKDDKVer.h>
	#include <Windows.h>
	#include <Winsock2.h>
	using socket_t = SOCKET;
	constexpr socket_t M_INVALID_SOCKET = INVALID_SOCKET;
#else //#elif defined(ENIGMA_PLATFORM_LINUX) || defined(ENIGMA_PLATFORM_MACOS)
	// Linux/Unix libraries will work in macos aswell.
	#include <netinet/in.h>
	using socket_t = Enigma::i32;
	constexpr socket_t M_INVALID_SOCKET = -1;
#endif


NS_ENIGMA_BEGIN
/**
*	@brief Single process instance interface
*	@details if you inherrit from this class, your application process will have only one instance at a time.
*   Using a socket implementation, this checks to see if the application is already open or not.
*   You must also choose a port that is unique to the application.
*/
class SingleProcessInstance
{
public:
	explicit SingleProcessInstance(const ui16 port) noexcept
		:
		m_socket_fd(M_INVALID_SOCKET),
		m_port(port),
		m_rc(1)
	{
#if defined(ENIGMA_PLATFORM_WINDOWS)
		// Windows Initialize WinSock
		WSAData data{};
		const auto err = WSAStartup(MAKEWORD(2, 2), &data);
		if (err != 0)
		{
			// https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
			auto err_reason = [err]() -> const char*
			{
				#define CASE_RET(c, v) case c: return v
				switch (err)
				{
					CASE_RET(WSASYSNOTREADY, "The underlying network subsystem is not ready for network communication.");
					CASE_RET(WSAVERNOTSUPPORTED, "The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.");
					CASE_RET(WSAEINPROGRESS, "A blocking Windows Sockets 1.1 operation is in progress.");
					CASE_RET(WSAEPROCLIM, "A limit on the number of tasks supported by the Windows Sockets implementation has been reached.");
					CASE_RET(WSAEFAULT, "The lpWSAData parameter is not a valid pointer.");
					default: return "<unknown winsock err>";
				}
				#undef CASE_RET
			};
		
			ENIGMA_ERROR("Failed to start WinSock: #{}: {}", err, err_reason());
			return;
		}
#endif

		// Create socket
		m_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
		if (m_socket_fd != M_INVALID_SOCKET)
		{
			// Bind to socket
			struct sockaddr_in name{};
			name.sin_family = AF_INET;
			name.sin_port = htons(port);
			name.sin_addr.s_addr = htonl(INADDR_ANY);
			m_rc = bind(m_socket_fd, (struct sockaddr*)&name, sizeof(name));
			if (m_rc < 0)
			{
				ENIGMA_ERROR("Failed to bind socket: {}", std::strerror(errno));
				return;
			}
		}
		else
		{
			ENIGMA_ERROR("Failed to create socket: {}", std::strerror(errno));
			return;
		}

		ENIGMA_INFO("Created Single Process Instance Socket: {} Port: {}", m_socket_fd, m_port);

	}

	/**
	*   Returns true if this is a unique process instance (no other instance process is running)
	*/
	constexpr bool IsUnique() const noexcept
	{
		return (m_socket_fd != M_INVALID_SOCKET) && (m_rc == 0); // valid socket and no binding errors.
	}

	virtual ~SingleProcessInstance() noexcept
	{
		if (m_socket_fd != M_INVALID_SOCKET)
		{
#if defined(ENIGMA_PLATFORM_WINDOWS)
			closesocket(m_socket_fd);
			WSACleanup(); // Shutdown WinSock
#else
			close(m_socket_fd);
#endif
			ENIGMA_INFO("Closed Single Process Instance Socket: {} Port: {}", m_socket_fd, m_port);
		}
	}

private:
	socket_t m_socket_fd{}; /**< Socket file descriptor, will be SOCKET in windows, int in linux */
	ui16 m_port{}; /**< Application's unique port */
	i32 m_rc{-1}; /**< Return code of bind socket function 0 means no error, when < 0 is an error */
};
NS_ENIGMA_END

#endif	// !ENIGMA_SINGLE_INSTANCE_H
