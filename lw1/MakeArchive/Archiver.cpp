#include "Archiver.h"
#include "Process.h"

void Archiver::Archive(const std::string& archiveName, std::vector<std::string>& files)
{
	std::vector<std::string> args = { "-cf", archiveName };
	args.insert(args.end(), files.begin(), files.end());

	Process p("tar", args);
	p.Wait();
}