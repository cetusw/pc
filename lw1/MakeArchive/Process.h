#ifndef MAKEARCHIVE_PROCCESS_H
#define MAKEARCHIVE_PROCCESS_H
#include <string>
#include <vector>

class Process
{
public:
	Process(const std::string& program, const std::vector<std::string>& args);
	void Wait();
	[[nodiscard]] pid_t GetPID() const;

private:
	static void ExecuteChild(const std::string& program, const std::vector<char*>& args);
	static std::vector<char*> ToCArgs(
		const std::string& program, const std::vector<std::string>& args);

	pid_t m_PID;
};

#endif // MAKEARCHIVE_PROCCESS_H