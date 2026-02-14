#ifndef MAKEARCHIVE_PARALLELCOMPRESSOR_H
#define MAKEARCHIVE_PARALLELCOMPRESSOR_H

#include "ICompressor.h"

class ParallelCompressor final : public ICompressor
{
public:
	explicit ParallelCompressor(int maxProcesses);

	std::vector<std::string> Compress(const std::vector<std::string>& files) override;

private:
	void WaitForSlot();
	void WaitAll();

	int m_maxProcesses;
	std::vector<pid_t> m_activePIDs;
};

#endif // MAKEARCHIVE_PARALLELCOMPRESSOR_H
