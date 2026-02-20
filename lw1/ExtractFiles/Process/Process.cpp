#include "../Process/Process.h"
#include <iostream>
#include <stdexcept>
#include <sys/wait.h>
#include <unistd.h>

Process::Process(
	const std::string& program, const std::vector<std::string>& args, Pipe* capturePipe)
{
	m_PID = fork();
	if (m_PID < 0)
	{
		throw std::runtime_error("fork failed");
	}

	if (m_PID == 0)
	{
		if (capturePipe)
		{
			capturePipe->CloseRead();
			dup2(capturePipe->GetWriteFileDescriptor(), STDOUT_FILENO);
			capturePipe->CloseWrite();
		}
		const auto cArgs = ToCArgs(program, args);
		ExecuteChild(program, cArgs);
	}

	if (m_PID > 0 && capturePipe)
	{
		capturePipe->CloseWrite();
		m_readFileDescriptor = capturePipe->GetReadFileDescriptor();
	}
}

Process::~Process()
{
	if (m_readFileDescriptor >= 0)
	{
		close(m_readFileDescriptor);
	}
}

std::vector<std::string> Process::ReadCapturedOutput() const
{
	if (m_readFileDescriptor < 0)
	{
		return {};
	}

	std::vector<std::string> output;
	std::string current;
	char buffer[1024];
	ssize_t bytesRead;
	while ((bytesRead = read(m_readFileDescriptor, buffer, 1024)) > 0)
	{
		for (int i = 0; i < bytesRead; i++)
		{
			if (buffer[i] == '\n')
			{
				output.push_back(current);
				current.clear();
				continue;
			}
			current += buffer[i];
		}
	}
	if (!current.empty())
	{
		output.push_back(current);
	}
	return output;
}

void Process::Wait()
{
	int status;
	if (m_PID <= 0)
	{
		return;
	}
	if (waitpid(m_PID, &status, 0) < 0)
	{
		throw std::runtime_error("waitpid failed");
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		throw std::runtime_error("Child process exited with error");
	}
	m_PID = -1;
}

pid_t Process::GetPID() const
{
	return m_PID;
}

void Process::ExecuteChild(const std::string& program, const std::vector<char*>& args)
{
	execvp(program.c_str(), args.data());
	std::cerr << "Failed to exec: " << program << "\n";
	std::exit(EXIT_FAILURE);
}

std::vector<char*> Process::ToCArgs(
	const std::string& program, const std::vector<std::string>& args)
{
	std::vector<char*> cArgs;
	cArgs.push_back(const_cast<char*>(program.c_str()));
	for (const auto& arg : args)
	{
		cArgs.push_back(const_cast<char*>(arg.c_str()));
	}
	cArgs.push_back(nullptr);
	return cArgs;
}
