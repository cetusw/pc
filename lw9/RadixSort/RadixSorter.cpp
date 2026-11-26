#include "RadixSorter.h"

namespace
{

std::size_t GetBlockCount(const std::size_t count, const std::size_t localSize)
{
	return (count + localSize - 1) / localSize;
}

} // namespace

RadixSorter::RadixSorter(
	OpenClContext& openCl,
	const OpenClProgram& program)
	: m_openCl(openCl)
	, m_markZerosKernel(program.CreateKernel("MarkZeros"))
	, m_scatterKernel(program.CreateKernel("Scatter"))
	, m_addBlockOffsetsKernel(program.CreateKernel("AddBlockOffsets"))
	, m_scanBlocksKernel(program.CreateKernel("ScanBlocks"))
{
}

std::vector<std::uint32_t> RadixSorter::Sort(const std::vector<std::uint32_t>& input)
{
	if (input.empty())
	{
		return {};
	}

	auto buffers = CreateSortBuffers(input);

	for (std::uint32_t bitIndex = 0; bitIndex < 32; ++bitIndex)
	{
		SortOneBitInBuffers(buffers, bitIndex, input.size());
	}

	std::vector<std::uint32_t> result(input.size());
	ReadBuffer(buffers.input, result);

	return result;
}

std::vector<std::int32_t> RadixSorter::Sort(const std::vector<std::int32_t>& input)
{
	if (input.empty())
	{
		return {};
	}
	
	std::vector<std::uint32_t> transformed(input.size());

	for (std::size_t i = 0; i < input.size(); ++i)
	{
		transformed[i] = static_cast<std::uint32_t>(input[i]) ^ 0x80000000u;
	}

	const std::vector<std::uint32_t> sorted = Sort(transformed);

	std::vector<std::int32_t> result(sorted.size());

	for (std::size_t i = 0; i < sorted.size(); ++i)
	{
		result[i] = static_cast<std::int32_t>(sorted[i] ^ 0x80000000u);
	}

	return result;
}

std::vector<std::uint32_t> RadixSorter::BuildZeroMask(
	const std::vector<std::uint32_t>& input,
	const std::uint32_t bitIndex)
{
	if (input.empty())
	{
		return {};
	}

	std::vector<std::uint32_t> isZero(input.size());

	const auto inputBuffer = CreateInputBuffer(input);
	const auto isZeroBuffer = CreateBuffer(input.size(), CL_MEM_READ_WRITE);

	RunMarkZerosKernel(inputBuffer, isZeroBuffer, bitIndex, input.size());
	ReadBuffer(isZeroBuffer, isZero);

	return isZero;
}

std::vector<std::uint32_t> RadixSorter::SortOneBit(const std::vector<std::uint32_t>& input, const std::uint32_t bitIndex)
{
	if (input.empty())
	{
		return {};
	}

	const auto isZero = BuildZeroMask(input, bitIndex);
	const auto zeroBefore = BuildExclusivePrefixSum(isZero);
	const auto totalZeros = zeroBefore.back() + isZero.back();

	const auto inputBuffer = CreateInputBuffer(input);
	const auto outputBuffer = CreateBuffer(input.size(), CL_MEM_READ_WRITE);
	const auto isZeroBuffer = CreateInputBuffer(isZero);
	const auto zeroBeforeBuffer = CreateInputBuffer(zeroBefore);

	RunScatterKernel(
		inputBuffer,
		outputBuffer,
		isZeroBuffer,
		zeroBeforeBuffer,
		totalZeros,
		input.size());

	std::vector<std::uint32_t> output(input.size());
	ReadBuffer(outputBuffer, output);

	return output;
}

cl::Buffer RadixSorter::CreateInputBuffer(const std::vector<std::uint32_t>& input) const
{
	return {
		m_openCl.GetContext(),
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		sizeof(std::uint32_t) * input.size(),
		// TODO почему const_cast. Пояснить в целом за поля
		const_cast<std::uint32_t*>(input.data())
	};
}

cl::Buffer RadixSorter::CreateBuffer(const std::size_t size, const cl_mem_flags flags) const
{
	return {
		m_openCl.GetContext(),
		flags,
		sizeof(std::uint32_t) * size
	};
}

RadixSortBuffers RadixSorter::CreateSortBuffers(const std::vector<std::uint32_t>& input) const
{
	const auto size = input.size();

	return {
		CreateInputBuffer(input),
		CreateBuffer(size, CL_MEM_READ_WRITE),
		CreateBuffer(size, CL_MEM_READ_WRITE),
		CreateBuffer(size, CL_MEM_READ_WRITE)
	};
}

void RadixSorter::RunMarkZerosKernel(
	const cl::Buffer& inputBuffer,
	const cl::Buffer& isZeroBuffer,
	const std::uint32_t bitIndex,
	const std::size_t count)
{
	m_markZerosKernel.setArg(0, inputBuffer);
	m_markZerosKernel.setArg(1, isZeroBuffer);
	m_markZerosKernel.setArg(2, bitIndex);
	m_markZerosKernel.setArg(3, static_cast<std::uint32_t>(count));

	// TODO что значит ND. поля
	m_openCl.GetQueue().enqueueNDRangeKernel(
		m_markZerosKernel,
		cl::NullRange,
		cl::NDRange(count),
		cl::NullRange);

	void(m_openCl.GetQueue().finish());
}

void RadixSorter::RunScatterKernel(
	const cl::Buffer& inputBuffer,
	const cl::Buffer& outputBuffer,
	const cl::Buffer& isZeroBuffer,
	const cl::Buffer& zeroBeforeBuffer,
	const std::uint32_t totalZeros,
	const std::size_t count)
{
	m_scatterKernel.setArg(0, inputBuffer);
	m_scatterKernel.setArg(1, outputBuffer);
	m_scatterKernel.setArg(2, isZeroBuffer);
	m_scatterKernel.setArg(3, zeroBeforeBuffer);
	m_scatterKernel.setArg(4, totalZeros);
	m_scatterKernel.setArg(5, static_cast<std::uint32_t>(count));

	m_openCl.GetQueue().enqueueNDRangeKernel(
		m_scatterKernel,
		cl::NullRange,
		cl::NDRange(count),
		cl::NullRange);
}

void RadixSorter::RunScanBlocksKernel(
	const cl::Buffer& inputBuffer,
	const cl::Buffer& outputBuffer,
	const cl::Buffer& blockSumsBuffer,
	std::size_t count,
	std::size_t localSize)
{
	const auto blockCount = GetBlockCount(count, localSize);
	const auto globalSize = blockCount * localSize;

	m_scanBlocksKernel.setArg(0, inputBuffer);
	m_scanBlocksKernel.setArg(1, outputBuffer);
	m_scanBlocksKernel.setArg(2, blockSumsBuffer);
	m_scanBlocksKernel.setArg(3, cl::Local(sizeof(std::uint32_t) * localSize));
	m_scanBlocksKernel.setArg(4, static_cast<std::uint32_t>(count));

	m_openCl.GetQueue().enqueueNDRangeKernel(
		m_scanBlocksKernel,
		cl::NullRange,
		cl::NDRange(globalSize),
		cl::NDRange(localSize));
}

void RadixSorter::RunAddBlockOffsetsKernel(
	const cl::Buffer& dataBuffer,
	const cl::Buffer& blockOffsetsBuffer,
	const std::size_t count,
	const std::size_t localSize)
{
	const auto blockCount = GetBlockCount(count, localSize);
	const auto globalSize = blockCount * localSize;

	m_addBlockOffsetsKernel.setArg(0, dataBuffer);
	m_addBlockOffsetsKernel.setArg(1, blockOffsetsBuffer);
	m_addBlockOffsetsKernel.setArg(2, static_cast<std::uint32_t>(count));

	m_openCl.GetQueue().enqueueNDRangeKernel(
		m_addBlockOffsetsKernel,
		cl::NullRange,
		cl::NDRange(globalSize),
		cl::NDRange(localSize));
}

void RadixSorter::RunExclusiveScan(
	const cl::Buffer& inputBuffer,
	const cl::Buffer& outputBuffer,
	const std::size_t count)
{
	constexpr std::size_t localSize = 1024; // TODO уточнить размер блока

	const auto blockCount = GetBlockCount(count, localSize);

	const auto blockSumsBuffer = CreateBuffer(blockCount, CL_MEM_READ_WRITE);
	const auto blockOffsetsBuffer = CreateBuffer(blockCount, CL_MEM_READ_WRITE);

	RunScanBlocksKernel(
		inputBuffer,
		outputBuffer,
		blockSumsBuffer,
		count,
		localSize);

	if (blockCount <= 1)
	{
		return;
	}

	RunExclusiveScan(blockSumsBuffer, blockOffsetsBuffer, blockCount);

	RunAddBlockOffsetsKernel(
		outputBuffer,
		blockOffsetsBuffer,
		count,
		localSize);
}

void RadixSorter::ReadBuffer(const cl::Buffer& buffer, std::vector<std::uint32_t>& output) const
{
	// TODO изучть все поля
	m_openCl.GetQueue().enqueueReadBuffer(
		buffer,
		CL_TRUE,
		0,
		sizeof(std::uint32_t) * output.size(),
		output.data());
}

std::uint32_t RadixSorter::ReadLast(const cl::Buffer& buffer, const std::size_t count) const
{
	std::uint32_t value = 0;

	m_openCl.GetQueue().enqueueReadBuffer(
		buffer,
		CL_TRUE,
		sizeof(std::uint32_t) * (count - 1),
		sizeof(std::uint32_t),
		&value);

	return value;
}

void RadixSorter::WriteBuffer(const cl::Buffer& buffer, const std::vector<std::uint32_t>& input) const
{
	m_openCl.GetQueue().enqueueWriteBuffer(
		buffer,
		CL_TRUE,
		0,
		sizeof(std::uint32_t) * input.size(),
		input.data());
}

void RadixSorter::SortOneBitInBuffers(RadixSortBuffers& buffers, const std::uint32_t bitIndex, const std::size_t count)
{
	// TODO можно вынести buffers в приватное поле
	RunMarkZerosKernel(buffers.input, buffers.isZero, bitIndex, count);

	RunExclusiveScan(
		buffers.isZero,
		buffers.zeroBefore,
		count);

	const auto totalZeros = ReadLast(buffers.zeroBefore, count) + ReadLast(buffers.isZero, count);

	RunScatterKernel(
		buffers.input,
		buffers.output,
		buffers.isZero,
		buffers.zeroBefore,
		totalZeros,
		count);

	std::swap(buffers.input, buffers.output);
}

std::vector<std::uint32_t> RadixSorter::BuildExclusivePrefixSum(const std::vector<std::uint32_t>& values)
{
	std::vector<std::uint32_t> result(values.size());

	std::uint32_t sum = 0;

	for (std::size_t i = 0; i < values.size(); ++i)
	{
		result[i] = sum;
		sum += values[i];
	}

	return result;
}