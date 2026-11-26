#include "OpenClProgram.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

OpenClProgram::OpenClProgram(
	const cl::Context& context,
	const cl::Device& device,
	const std::filesystem::path& sourcePath)
	: m_program(context, ReadSourceFile(sourcePath))
{
	try
	{
		m_program.build({ device });
	}
	catch (...)
	{
		const auto log = m_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
		throw std::runtime_error("Program build error:\n" + log);
	}
}

cl::Kernel OpenClProgram::CreateKernel(const char* name) const
{
	return { m_program, name };
}

std::string OpenClProgram::ReadSourceFile(const std::filesystem::path& path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open kernel file: " + path.string());
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}