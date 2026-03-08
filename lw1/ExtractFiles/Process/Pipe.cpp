#include "Pipe.h"
#include <stdexcept>
#include <unistd.h>

Pipe::Pipe()
{
	if (pipe(m_fileDescriptors) < 0)
	{
		throw std::runtime_error("pipe failed");
	}
}

Pipe::~Pipe()
{
	CloseRead();
	CloseWrite();
}

int Pipe::GetReadFileDescriptor() const
{
	return m_fileDescriptors[0];
}

int Pipe::GetWriteFileDescriptor() const
{
	return m_fileDescriptors[1];
}

void Pipe::CloseRead()
{
	if (m_fileDescriptors[0] >= 0)
	{
		close(m_fileDescriptors[0]);
		m_fileDescriptors[0] = -1;
	}
}

void Pipe::CloseWrite()
{
	if (m_fileDescriptors[1] >= 0)
	{
		close(m_fileDescriptors[1]);
		m_fileDescriptors[1] = -1;
	}
}