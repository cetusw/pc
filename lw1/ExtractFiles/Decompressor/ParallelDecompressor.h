#pragma once

#include "IDecompressor.h"

class ParallelDecompressor final : public IDecompressor
{
public:
	explicit ParallelDecompressor(int maxProcesses);
	std::vector<std::string> Decompress(const std::vector<std::string>& files) override;

private:
	void WaitForSlot();
	void WaitAll();

	int m_maxProcesses;
	std::vector<pid_t> m_activePIDs;
};
