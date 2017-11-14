#pragma once

#include "json/json.h"
#include <memory>

namespace Json
{
	class StreamWriter;
}

class JSONQueryWriter
{
public:
	JSONQueryWriter();

	template<class T>
	void add( const char* name, const T& val )
	{
		m_root[name] = val;
	}

	std::string str();

private:
	Json::Value							m_root;
	std::unique_ptr<Json::StreamWriter> m_writer;
};

class JSONQueryReader
{
public:
	JSONQueryReader(const std::string& str);
	JSONQueryReader(const Json::Value& value);

	template<typename T>
	T get(const char* name) const;
	template<typename T>
	T get() const;

	std::vector<JSONQueryReader> getArray(const char* name) const;

	bool isValid() const { return m_valid; }
private:
	Json::Value	m_root;
	bool		m_valid;
};



