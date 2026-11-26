#pragma once

#include <CL/opencl.hpp>

class OpenClContext
{
public:
	OpenClContext();

	[[nodiscard]] const cl::Device& GetDevice() const;
	[[nodiscard]] const cl::Context& GetContext() const;
	[[nodiscard]] cl::CommandQueue& GetQueue();

private:
	static cl::Device SelectDevice();

	cl::Device m_device;
	cl::Context m_context;
	cl::CommandQueue m_queue;
};