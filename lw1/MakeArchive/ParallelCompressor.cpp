#include "ParallelCompressor.h"

#include "FileUtils.h"
#include "Process.h"

#include <stdexcept>
#include <sys/wait.h>

ParallelCompressor::ParallelCompressor(const int maxProcesses)
	: m_maxProcesses(maxProcesses)
{
}

std::vector<std::string> ParallelCompressor::Compress(const std::vector<std::string>& files)
{
	std::vector<std::string> gzFiles;

	for (const auto& file : files)
	{
		if (m_activePIDs.size() >= static_cast<size_t>(m_maxProcesses))
		{
			WaitForSlot();
		}

		Process p("gzip", { "-k", "-f", file });
		m_activePIDs.push_back(p.GetPID());

		gzFiles.push_back(FileUtils::GetGzName(file));
	}
	WaitAll();
	return gzFiles;
}

void ParallelCompressor::WaitForSlot()
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

void ParallelCompressor::WaitAll()
{
	while (!m_activePIDs.empty())
	{
		WaitForSlot();
	}
}