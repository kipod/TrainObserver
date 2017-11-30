#include "connection_manager.h"
#include "log_interface.h"

#include <winsock2.h>
#include <Ws2tcpip.h>


const uint WSA_VERSION_LO = 2;
const uint WSA_VERSION_HI = 2;
const uint READ_BUFFER_SIZE = 1000;

ConnectionManager::ConnectionManager():
	m_initialized(false),
	m_socket(INVALID_SOCKET)
{

}

ConnectionManager::~ConnectionManager()
{
	reset();
}

bool ConnectionManager::init()
{
	reset();

	WSADATA wsaData;

	int err = WSAStartup(MAKEWORD(WSA_VERSION_LO, WSA_VERSION_HI), &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		LOG(MSG_ERROR, "WSAStartup failed with error: %d", err);
		return false;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != WSA_VERSION_LO || HIBYTE(wsaData.wVersion) != WSA_VERSION_HI) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		LOG(MSG_ERROR, "Could not find a usable version of Winsock.dll");
		WSACleanup();
		return false;
	}
	else
		LOG(MSG_NORMAL, "The Winsock %d.%d dll was found okay", WSA_VERSION_HI, WSA_VERSION_LO);

	m_initialized = true;

	if (!createSocket())
	{
		WSACleanup();
		m_initialized = false;
		return false;
	}

	return true;
}

void ConnectionManager::reset()
{
	if (m_initialized)
	{
		closeSocket();
		WSACleanup();
		m_initialized = false;
	}
}

bool ConnectionManager::connect(const char* servername, uint16_t portNumber)
{
	if (!m_initialized)
	{
		LOG(MSG_ERROR, "Trying to connect with uninitialized WSA");
		return false;
	}

	if (!initAddr(servername, portNumber))
	{
		return false;
	}
	
	/*---Connect to server---*/
	int result = ::connect(m_socket, (sockaddr*)m_addr.get(), sizeof(sockaddr_in));
	if (result == SOCKET_ERROR)
	{
		LOG(MSG_ERROR, "connect function failed with error: %ld", WSAGetLastError());
		result = closesocket(m_socket);
		if (result == SOCKET_ERROR)
		{
			LOG(MSG_ERROR, "closesocket function failed with error: %ld", WSAGetLastError());
		}
		return false;
	}

	LOG(MSG_NORMAL, "Connected to server %s:%d.", servername, portNumber);
	return true;
}

bool ConnectionManager::sendMessage(Action actionCode, const std::string* message) const
{
	size_t msgLength = message ? message->length() : 0;
	if (msgLength > 0)
	{
		size_t fullLength = msgLength + sizeof(ActionMessageHeader);
		auto* action = reinterpret_cast<ActionMessage *>(alloca(fullLength)); // stack allocation memory
		action->header.actionCode = actionCode;
		action->header.dataLength = msgLength;
		::memcpy(action->buffer, message->c_str(), msgLength);
		return send(action, fullLength);
	}
	else
	{
		ActionMessageHeader header = { actionCode, 0 };
		return send(&header, sizeof(header));
	}
}

Result ConnectionManager::receiveMessage(std::string& message) const
{
	if (!m_initialized || m_socket == INVALID_SOCKET)
	{
		LOG(MSG_ERROR, "Trying to receive message with uninitialized WSA");
		return Result::SOCKET_UNINITIALIZED;
	}

	Result result = SOCKET_UNINITIALIZED;
	if (!receive(result))
	{
		return Result::INCORRECT_RESPOND_FORMAT;
	}

	uint length = 0;
	if (!receive(length))
	{
		return Result::INCORRECT_RESPOND_FORMAT;
	}

	if (length > 0)
	{
		message.resize(length);

		uint receivedBytes = 0;
		char buf[READ_BUFFER_SIZE];
		while (receivedBytes < length)
		{
			int bytesToReceive = min(READ_BUFFER_SIZE, length - receivedBytes);
			int n = receive(buf, bytesToReceive);

			if (n < 0)
			{
				return Result::SOCKET_ERR;
			}

			memcpy_s((char*)(message.data() + receivedBytes), n, buf, n);
			receivedBytes += n;
		}
	}
	else
	{
		message = "";
	}

	return result;	
}

bool ConnectionManager::send(const void* buf, size_t nbytes) const
{
	if (!m_initialized)
	{
		LOG(MSG_ERROR, "Trying to send message with uninitialized WSA");
		return false;
	}

	if (!m_addr)
	{
		LOG(MSG_ERROR, "Trying to send message to disconnected server");
		return false;
	}

	size_t bytesSent = 0;
	do {
		int result = ::send(m_socket, (const char*)buf, (nbytes - bytesSent), 0);
		if (result == SOCKET_ERROR)
		{
			LOG(MSG_ERROR, "send of message failed!");
			break;
		}
		bytesSent += result;
	}
	while (bytesSent < nbytes);

	

	return bytesSent == nbytes;
}

int ConnectionManager::receive(char* buf, uint length) const
{
	if (!m_initialized || m_socket == INVALID_SOCKET)
	{
		LOG(MSG_ERROR, "Trying to receive message with uninitialized WSA");
		return -1;
	}

	int n = ::recv(m_socket, buf, length, 0);
	if (n < 0)
	{
		LOG(MSG_ERROR, "SOCKET_ERROR while receiving message");
		//if (WSAGetLastError() != WSAEWOULDBLOCK)
		//{
		//	closeSocket();
		//}
	}

	return n;
}

void ConnectionManager::closeSocket()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
}

bool ConnectionManager::createSocket()
{
	if (!m_initialized)
	{
		LOG(MSG_ERROR, "Trying to create a socket with uninitialized WSA");
		return false;
	}

	/*---Open socket for streaming---*/
	if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		LOG(MSG_ERROR, "socket function failed with error: %ld\n", WSAGetLastError());
		return false;
	}

	return true;
}

bool ConnectionManager::initAddr(const char* servername, uint16_t portNumber)
{
	/*---Initialize server address/port struct---*/
	m_addr.reset(new sockaddr_in());
	ZeroMemory(m_addr.get(), sizeof(sockaddr_in));
	m_addr->sin_family = AF_INET;
	m_addr->sin_port = htons(portNumber);
	//dest.sin_addr.s_addr = ::inet_addr(SERVER_ADDR);
	if (InetPton(AF_INET, servername, &m_addr->sin_addr) != 1)
	{
		LOG(MSG_ERROR, "Could not resolve server name: \"%s\"", servername);
		return false;
	}

	return true;
}

