#pragma once

#include <CL/opencl.hpp>

#include <filesystem>

class OpenClProgram
{
public:
	OpenClProgram(
		const cl::Context& context,
		const cl::Device& device,
		const std::filesystem::path& sourcePath);

	[[nodiscard]] cl::Kernel CreateKernel(const char* name) const;

private:
	static std::string ReadSourceFile(const std::filesystem::path& path);

	cl::Program m_program;
};