#pragma once

#include "OpenClContext.h"
#include "OpenClProgram.h"

#include <vector>

struct RadixSortBuffers
{
	cl::Buffer input;
	cl::Buffer output;
	cl::Buffer isZero;
	cl::Buffer zeroBefore;
};

class RadixSorter
{
public:
	RadixSorter(OpenClContext& openCl, const OpenClProgram& program);

	[[nodiscard]] std::vector<std::uint32_t> Sort(const std::vector<std::uint32_t>& input);

	std::vector<std::int32_t> Sort(const std::vector<std::int32_t>& input);

	[[nodiscard]] std::vector<std::uint32_t> BuildZeroMask(
		const std::vector<std::uint32_t>& input,
		std::uint32_t bitIndex);

	std::vector<std::uint32_t> SortOneBit(const std::vector<std::uint32_t>& input, std::uint32_t bitIndex);

private:
	[[nodiscard]] cl::Buffer CreateInputBuffer(const std::vector<std::uint32_t>& input) const;

	[[nodiscard]] cl::Buffer CreateBuffer(size_t size, cl_mem_flags flags) const;

	[[nodiscard]] RadixSortBuffers CreateSortBuffers(const std::vector<std::uint32_t>& input) const;

	void RunMarkZerosKernel(
		const cl::Buffer& inputBuffer,
		const cl::Buffer& isZeroBuffer,
		std::uint32_t bitIndex,
		std::size_t count);

	void RunScatterKernel(
		const cl::Buffer& inputBuffer,
		const cl::Buffer& outputBuffer,
		const cl::Buffer& isZeroBuffer,
		const cl::Buffer& zeroBeforeBuffer,
		std::uint32_t totalZeros,
		std::size_t count);

	void RunScanBlocksKernel(
		const cl::Buffer& inputBuffer,
		const cl::Buffer& outputBuffer,
		const cl::Buffer& blockSumsBuffer,
		std::size_t count,
		std::size_t localSize);

	void RunAddBlockOffsetsKernel(
		const cl::Buffer& dataBuffer,
		const cl::Buffer& blockOffsetsBuffer,
		std::size_t count,
		std::size_t localSize);

	void RunExclusiveScan(
		const cl::Buffer& inputBuffer,
		const cl::Buffer& outputBuffer,
		std::size_t count);

	void ReadBuffer(const cl::Buffer& buffer, std::vector<std::uint32_t>& output) const;

	[[nodiscard]] std::uint32_t ReadLast(const cl::Buffer& buffer, std::size_t count) const;

	void WriteBuffer(const cl::Buffer& buffer, const std::vector<std::uint32_t>& input) const;

	void SortOneBitInBuffers(RadixSortBuffers& buffers, std::uint32_t bitIndex, std::size_t count);

	[[nodiscard]] static std::vector<std::uint32_t> BuildExclusivePrefixSum(const std::vector<std::uint32_t>& values);

	OpenClContext& m_openCl;
	cl::Kernel m_markZerosKernel;
	cl::Kernel m_scatterKernel;
	cl::Kernel m_addBlockOffsetsKernel;
	cl::Kernel m_scanBlocksKernel;
};