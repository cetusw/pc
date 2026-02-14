#ifndef MAKEARCHIVE_ICOMPRESSOR_H
#define MAKEARCHIVE_ICOMPRESSOR_H

#include <string>
#include <vector>

class ICompressor
{
public:
	virtual ~ICompressor() = default;
	virtual std::vector<std::string> Compress(const std::vector<std::string>& files) = 0;
};

#endif // MAKEARCHIVE_ICOMPRESSOR_H
