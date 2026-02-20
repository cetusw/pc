#include "../Decompressor/SequentialDecompressor.h"
#include "../Process/Process.h"

std::vector<std::string> SequentialDecompressor::Decompress(const std::vector<std::string>& files)
{
	std::vector<std::string> decompressedFiles;
	for (const auto& file : files)
	{
		Process p("gzip", { "-d", "-f", file });
		p.Wait();
		decompressedFiles.push_back(file);
	}
	return decompressedFiles;
}
