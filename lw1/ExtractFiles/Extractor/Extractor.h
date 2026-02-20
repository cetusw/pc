#pragma once

#include <string>
#include <vector>

class Extractor
{
public:
	static std::vector<std::string> Extract(
		const std::string& archiveName, const std::string& outputDirectory);

private:
	static void EnsureDirectoryExists(const std::string& directory);
	static std::vector<std::string> FormArguments(
		const std::string& archiveName, const std::string& outputDirectory);
	static std::vector<std::string> NormalizePaths(const std::string& outputDirectory, const std::vector<std::string>& extractedFiles);
};
