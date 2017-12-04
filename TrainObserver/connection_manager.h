#pragma once
#include <memory>
#include "defs.hpp"
#include <string>


class ConnectionManager
{
public:
	ConnectionManager();
	virtual ~ConnectionManager();
	ConnectionManager(const ConnectionManager&) = delete;
	ConnectionManager(ConnectionManager&&) = delete;


	bool init();
	void reset();

	bool connect(const char* servername, uint16_t portNumber);
	bool sendMessage(Action actionCode, bool needResponce = false, const std::string* message = nullptr) const;
	Result receiveMessage(std::string& message) const;

private:
	void closeSocket();
	bool createSocket();
	bool initAddr(const char* servername, uint16_t portNumber);
	bool send(const void* buf, size_t nbytes) const;
	int receive(char* buf, uint length) const;

	template<class T>
	bool receive(T& val) const
	{
		int n = receive((char*)&val, sizeof(val));
		return n == sizeof(val);
	}

private:
	uint								m_socket;
	std::unique_ptr<struct sockaddr_in>	m_addr;
	bool								m_initialized;
};