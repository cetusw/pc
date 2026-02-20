#pragma once

#include "IDecompressor.h"

class SequentialDecompressor final : public IDecompressor
{
public:
	std::vector<std::string> Decompress(const std::vector<std::string>& files) override;
};
