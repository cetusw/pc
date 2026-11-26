#include "OpenClContext.h"

#include <iostream>

OpenClContext::OpenClContext()
	: m_device(SelectDevice())
	, m_context(m_device)
	, m_queue(m_context, m_device)
{
}

const cl::Device& OpenClContext::GetDevice() const
{
	return m_device;
}

const cl::Context& OpenClContext::GetContext() const
{
	return m_context;
}

cl::CommandQueue& OpenClContext::GetQueue()
{
	return m_queue;
}

cl::Device OpenClContext::SelectDevice()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	if (platforms.empty())
	{
		throw std::runtime_error("No OpenCL platforms found");
	}

	for (const auto& platform : platforms)
	{
		std::vector<cl::Device> devices;
		platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

		if (!devices.empty())
		{
			return std::move(devices.front());
		}
	}

	throw std::runtime_error("No GPU devices found");
}