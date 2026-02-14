#include "SequentialCompressor.h"
#include "FileUtils.h"
#include "Process.h"

std::vector<std::string> SequentialCompressor::Compress(const std::vector<std::string>& files)
{
	std::vector<std::string> gzFiles;
	for (const auto& file : files)
	{
		Process p("gzip", { "-k", "-f", file });
		p.Wait();
		gzFiles.push_back(FileUtils::GetGzName(file));
	}
	return gzFiles;
}