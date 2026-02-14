#ifndef MAKEARCHIVE_FILEUTILS_H
#define MAKEARCHIVE_FILEUTILS_H

#pragma once
#include <string>

namespace FileUtils
{
inline std::string GetGzName(const std::string& file)
{
	return file + ".gz";
}
} // namespace FileUtils

#endif // MAKEARCHIVE_FILEUTILS_H
