#include "FileDescriptor.h"

#include <stdexcept>

FileDescriptor::FileDescriptor(const std::string& filename, const char* mode)
{
	m_file = fopen(filename.c_str(), mode);
	if (!m_file)
	{
		throw std::runtime_error("Failed to open file");
	}
}

FileDescriptor::~FileDescriptor()
{
	if (m_file)
	{
		fclose(m_file);
	}
}

FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept
	: m_file(other.m_file)
{
	other.m_file = nullptr;
}

FILE* FileDescriptor::Get() const
{
	return m_file;
}