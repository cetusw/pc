__kernel void MarkZeros(
    __global const uint* input,
    __global uint* isZero,
    uint bitIndex,
    uint count)
{
    uint id = get_global_id(0);

    if (id >= count)
    {
        return;
    }

    uint value = input[id];
    uint bit = (value >> bitIndex) & 1u;

    isZero[id] = bit == 0u ? 1u : 0u;
}

__kernel void Scatter(
    __global const uint* input,
    __global uint* output,
    __global const uint* isZero,
    __global const uint* zeroBefore,
    uint totalZeros,
    uint count)
{
    uint id = get_global_id(0);

    if (id >= count)
    {
        return;
    }

    uint newIndex;

    if (isZero[id])
    {
        newIndex = zeroBefore[id];
    }
    else
    {
        uint onesBefore = id - zeroBefore[id];
        newIndex = totalZeros + onesBefore;
    }

    output[newIndex] = input[id];
}

uint LoadOrZero(
    __global const uint* input,
    uint globalId,
    uint count)
{
    return globalId < count ? input[globalId] : 0u;
}

uint ReadLeftValue(
    __local const uint* localData,
    uint localId,
    uint offset)
{
    return localId >= offset ? localData[localId - offset] : 0u;
}

uint ToExclusiveValue(
    __local const uint* localData,
    uint localId)
{
    return localId == 0 ? 0u : localData[localId - 1];
}

__kernel void ScanBlocks(
    __global const uint* input,
    __global uint* output,
    __global uint* blockSums,
    __local uint* localData,
    uint count)
{
    uint globalId = get_global_id(0);
    uint localId = get_local_id(0);
    uint groupId = get_group_id(0);
    uint localSize = get_local_size(0);

    localData[localId] = LoadOrZero(input, globalId, count);

    barrier(CLK_LOCAL_MEM_FENCE);

    for (uint offset = 1; offset < localSize; offset <<= 1)
    {
        uint addValue = ReadLeftValue(localData, localId, offset);

        barrier(CLK_LOCAL_MEM_FENCE);

        localData[localId] += addValue;

        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if (globalId < count)
    {
        output[globalId] = ToExclusiveValue(localData, localId);
    }

    if (localId == localSize - 1)
    {
        blockSums[groupId] = localData[localId];
    }
}

__kernel void AddBlockOffsets(
    __global uint* data,
    __global const uint* blockOffsets,
    uint count)
{
    uint globalId = get_global_id(0);

    if (globalId >= count)
    {
        return;
    }

    uint groupId = get_group_id(0);
    data[globalId] += blockOffsets[groupId];
}