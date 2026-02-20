#pragma once

#include <string>
#include <vector>

class IDecompressor
{
public:
	virtual ~IDecompressor() = default;
	virtual std::vector<std::string> Decompress(const std::vector<std::string>& files) = 0;
};
