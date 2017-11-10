#pragma once
#include <memory>
#include "defs.hpp"
#include <string>


class ConnectionManager
{
public:
	ConnectionManager();
	~ConnectionManager();


	bool init();
	void reset();

	bool connect(const wchar_t* servername, int portNumber);
	bool sendMessage(Action actionCode, const std::string* message) const;
	Result receiveMessage(std::string& message) const;

private:
	void closeSocket();
	bool createSocket();
	bool initAddr(const wchar_t* servername, int portNumber);
	bool send(const void* buf, int nbytes) const;
	int receive(char* buf, unsigned int length) const;

	template<class T>
	bool receive(T& val) const
	{
		int n = receive((char*)&val, sizeof(val));
		return n == sizeof(val);
	}

private:
	unsigned int						m_socket;
	std::unique_ptr<struct sockaddr_in>	m_addr;
	bool								m_initialized;
};