#include "HartEng/Core/Buffer.h"

#include "HartEng/Core/Core.h"

#include <cstdint>
#include <iostream>
namespace HE
{
	Buffer::Buffer() :
		Size(0)
	{
	}

	Buffer::Buffer(uint8_t* data, uint32_t size) :
		Size(size)
	{
		Data.resize(size);
		for (int i = 0; i < size; i++)
		{
			Data[i] = data[i];
		}
	}

	Buffer::~Buffer()
	{

	}

	void Buffer::Allocate(uint32_t size)
	{
		if (Size)
		{
			Data.clear();
			Size = 0;
		}


		if (size == 0)
			return;

		Data.resize(size);
		Size = size;
	}

	void Buffer::ZeroInitialize()
	{
		if (Size)
			memset(&Data[0], 0, Size);
	}

	void Buffer::Write(void* data, uint32_t size, uint32_t offset)
	{
		HE_CORE_ASSERT(offset + size <= Size, "Buffer overflow!");
		memcpy(&Data[0] + offset, data, size);
	}
}