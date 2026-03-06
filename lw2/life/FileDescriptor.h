#pragma once
#include <string>

class FileDescriptor
{
public:
	FileDescriptor(const std::string& filename, const char* mode);
	~FileDescriptor();

	FileDescriptor(const FileDescriptor&) = delete;
	FileDescriptor& operator=(const FileDescriptor&) = delete;

	FileDescriptor(FileDescriptor&& other) noexcept;

	FILE* Get() const;

private:
	FILE* m_file;
};