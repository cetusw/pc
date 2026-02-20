#include "ParallelDecompressor.h"
#include "../Process/Process.h"
#include <stdexcept>
#include <sys/wait.h>

ParallelDecompressor::ParallelDecompressor(const int maxProcesses)
	: m_maxProcesses(maxProcesses)
{
}

std::vector<std::string> ParallelDecompressor::Decompress(const std::vector<std::string>& files)
{
	std::vector<std::string> decompressedFiles;

	for (const auto& file : files)
	{
		if (m_activePIDs.size() >= static_cast<size_t>(m_maxProcesses))
		{
			WaitForSlot();
		}

		Process p("gzip", { "-d", "-f", file });
		m_activePIDs.push_back(p.GetPID());

		decompressedFiles.push_back(file);
	}
	WaitAll();
	return decompressedFiles;
}

void ParallelDecompressor::WaitForSlot()
{
	int status;

	const pid_t donePID = waitpid(-1, &status, 0);
	if (donePID < 0)
	{
		throw std::runtime_error("waitpid failed");
	}

	std::erase(m_activePIDs, donePID);

	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		throw std::runtime_error("One of the compression processes failed");
	}
}

void ParallelDecompressor::WaitAll()
{
	while (!m_activePIDs.empty())
	{
		WaitForSlot();
	}
}