#pragma once

#include "Pipe.h"
#include <string>
#include <vector>

class Process
{
public:
	Process(const std::string& program, const std::vector<std::string>& args, Pipe* capturePipe = nullptr);
	~Process();

	[[nodiscard]] std::vector<std::string> ReadCapturedOutput() const;

	void Wait();

	[[nodiscard]] pid_t GetPID() const;

private:
	static void ExecuteChild(const std::string& program, const std::vector<char*>& args);

	static std::vector<char*> ToCArgs(
		const std::string& program, const std::vector<std::string>& args);

	pid_t m_PID = -1;
	int m_readFileDescriptor = -1;
};
