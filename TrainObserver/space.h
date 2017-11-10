#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct Line;
class ConnectionManager;
class JSONQueryReader;

struct Point
{
	unsigned int idx;
	unsigned int post_id;

	std::vector<const Line*> lines;

	Point(unsigned int idx_, unsigned int post_id_) :
		idx(idx_),
		post_id(post_id_)
	{}

};

struct Line
{
	unsigned int idx;
	unsigned int length;
	unsigned int pid_1;
	unsigned int pid_2;

	Point*		pt_1;
	Point*		pt_2;

	Line(unsigned int idx_, unsigned int length_, unsigned int pid1, unsigned int pid2):
		idx(idx_),
		length(length_),
		pid_1(pid1),
		pid_2(pid2)
	{}
};


class Space
{
public:
	Space();
	~Space();

	bool init(const ConnectionManager& manager);

private:
	bool loadLines(const JSONQueryReader& reader);
	bool loadPoints(const JSONQueryReader& reader);
private:
	unsigned int	m_idx;
	std::string		m_name;

	std::unordered_map<unsigned int, Point> m_points;
	std::unordered_map<unsigned int, Line> m_lines;

	bool			m_valid;
};

