#include "Extractor.h"
#include "../Process/Process.h"
#include <filesystem>

std::vector<std::string> Extractor::Extract(
	const std::string& archiveName, const std::string& outputDirectory)
{
	EnsureDirectoryExists(outputDirectory);
	const auto args = FormArguments(archiveName, outputDirectory);

	Pipe pipe;
	Process p("tar", args, &pipe);
	const std::vector<std::string> extractedFiles = p.ReadCapturedOutput();
	p.Wait();

	return NormalizePaths(outputDirectory, extractedFiles);
}

void Extractor::EnsureDirectoryExists(const std::string& directory)
{
	if (!directory.empty() && !std::filesystem::exists(directory))
	{
		std::filesystem::create_directories(directory);
	}
}

std::vector<std::string> Extractor::FormArguments(
	const std::string& archiveName, const std::string& outputDirectory)
{
	std::vector<std::string> args = { "-xvf", archiveName };

	if (!outputDirectory.empty())
	{
		args.emplace_back("-C");
		args.push_back(outputDirectory);
	}

	return args;
}

std::vector<std::string> Extractor::NormalizePaths(
	const std::string& outputDirectory, const std::vector<std::string>& extractedFiles)
{
	if (outputDirectory.empty())
	{
		return extractedFiles;
	}

	std::vector<std::string> normalized;
	normalized.reserve(extractedFiles.size());

	const std::filesystem::path base(outputDirectory);

	for (const auto& file : extractedFiles)
	{
		normalized.push_back((base / file).string());
	}

	return normalized;
}
