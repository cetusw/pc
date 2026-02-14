#ifndef MAKEARCHIVE_SEQUENTIALCOMPRESSOR_H
#define MAKEARCHIVE_SEQUENTIALCOMPRESSOR_H

#include "ICompressor.h"

class SequentialCompressor final : public ICompressor
{
public:
	std::vector<std::string> Compress(const std::vector<std::string>& files) override;
};

#endif // MAKEARCHIVE_SEQUENTIALCOMPRESSOR_H
