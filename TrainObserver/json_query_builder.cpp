#include "json_query_builder.h"
#include "defs.hpp"

#include <iosfwd>
#include <sstream>


JSONQueryWriter::JSONQueryWriter()
{
	Json::StreamWriterBuilder writerBuilder;
	m_writer.reset(writerBuilder.newStreamWriter());
}

std::string JSONQueryWriter::str()
{
	std::ostringstream ss;
	m_writer->write(m_root, &ss);

	m_root.clear();
	return ss.str();
}

//////////////////////////////////////////////////////////////////////////

JSONQueryReader::JSONQueryReader(const std::string& str)
{
	Json::CharReaderBuilder readerBuilder;
	std::unique_ptr<Json::CharReader>	reader(readerBuilder.newCharReader());

	std::string errs;
	m_valid = reader->parse(str.c_str(), str.c_str() + str.length(), &m_root, &errs);
}

JSONQueryReader::JSONQueryReader(const Json::Value& value):
	m_root(value)
{
	m_valid = true;
}


std::vector<JSONQueryReader> JSONQueryReader::getArray(const char* name) const
{
	std::vector<JSONQueryReader> res;
	const Json::Value& val = m_root[name];
	if (!val.isNull())
	{
		for (auto it = val.begin(); it != val.end(); ++it)
		{
			res.emplace_back(*it);
		}
	}

	return res;
}

template<>
int JSONQueryReader::get<int>(const char* name) const
{
	return m_root[name].asInt();
}

template<>
uint JSONQueryReader::get<uint>(const char* name) const
{
	return m_root[name].asUInt();
}

template<>
float JSONQueryReader::get<float>(const char* name) const
{
	return m_root[name].asFloat();
}

template<>
std::string JSONQueryReader::get<std::string>(const char* name) const
{
	return m_root[name].asString();
}

template<>
bool JSONQueryReader::get<bool>(const char* name) const
{
	return m_root[name].asBool();
}

//////////////////////////////////////////////////////////////////////////

template<>
int JSONQueryReader::get<int>() const
{
	return m_root.asInt();
}

template<>
unsigned int JSONQueryReader::get<unsigned int>() const
{
	return m_root.asUInt();
}

template<>
float JSONQueryReader::get<float>() const
{
	return m_root.asFloat();
}

template<>
std::string JSONQueryReader::get<std::string>() const
{
	return m_root.asString();
}

template<>
bool JSONQueryReader::get<bool>() const
{
	return m_root.asBool();
}