#pragma once

class Pipe
{
public:
	Pipe();
	~Pipe();

	Pipe(const Pipe&) = delete;
	Pipe& operator=(const Pipe&) = delete;

	[[nodiscard]] int GetReadFileDescriptor() const;
	[[nodiscard]] int GetWriteFileDescriptor() const;

	void CloseRead();
	void CloseWrite();

private:
	int m_fileDescriptors[2] = { -1, -1 };
};