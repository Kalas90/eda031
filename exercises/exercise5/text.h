#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>

class Text {
public:
	void addLine(const std::string& line) { lines.push_back(line); }
private:
	std::vector<std::string> lines;
};

#endif
