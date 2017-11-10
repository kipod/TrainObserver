#pragma once
#include <stdint.h>

enum Action : uint32_t
{
	LOGIN = 1,
	LOGOUT = 2,
	MOVE = 3,
	TURN = 5,
	MAP = 10
};

enum Result : uint32_t
{
	OKEY					= 0,
	BAD_COMMAND				= 1,
	RESOURCE_NOT_FOUND		= 2,
	PATH_NOT_FOUND			= 3,
	ACCESS_DENIED			= 5,

	SOCKET_ERR				= 0xfe000001,
	SOCKET_UNINITIALIZED	= 0xfe000002,

	INCORRECT_RESPOND_FORMAT= 0xff000001
};

struct ActionMessageHeader
{
    Action actionCode;
    size_t dataLength;
};
//
//struct ResposeMessage
//{
//    Result result;
//    size_t dataLength;
//    char data[];
//}