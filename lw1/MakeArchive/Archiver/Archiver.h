#ifndef MAKEARCHIVE_ARCHIVER_H
#define MAKEARCHIVE_ARCHIVER_H
#include <string>
#include <vector>

class Archiver
{
public:
	static void Archive(const std::string& archiveName, std::vector<std::string>& files);
};

#endif // MAKEARCHIVE_ARCHIVER_H
