#pragma once

#include <stdint.h>
#include <vector>

namespace HE 
{
	class Buffer
	{
	public:
		std::vector<uint8_t> Data;
		uint32_t Size;

		Buffer();
		Buffer(uint8_t* data, uint32_t size);
		virtual ~Buffer();
		void Allocate(uint32_t size);
		void ZeroInitialize();
		void Write(void* data, uint32_t size, uint32_t offset = 0);
		inline uint32_t GetSize() const { return Size; }


		template<typename T>
		T& Read(uint32_t offset = 0)
		{
			return *(T*)(&Data[0] + offset);
		}

		template<typename T>
		T* As()
		{
			return (T*)&Data[0];
		}

		operator bool() const
		{
			return Size;
		}

		uint8_t& operator[](int index)
		{
			return Data[index];
		}
		/*
		operator void*()
		{
			return &Data[0];
		}

		operator const void* () const
		{
			return &Data[0];
		}
		*/

		uint8_t operator[](int index) const
		{
			return Data[index];
		}


	};
	/*
	class Buffer
	{
	public:
		uint8_t* Data = nullptr;
		uint32_t Size;

		Buffer();

		Buffer(uint8_t* data, uint32_t size);

		~Buffer();

		//static Buffer Copy(void* data, uint32_t size);

		void Allocate(uint32_t size);

		void ZeroInitialize();

		void Write(void* data, uint32_t size, uint32_t offset = 0);

		inline uint32_t GetSize() const { return Size; }

		template<typename T>
		T& Read(uint32_t offset = 0)
		{
			return *(T*)(Data + offset);
		}

		template<typename T>
		T* As()
		{
			return (T*)Data;
		}

		operator bool() const
		{
			return Data;
		}

		uint8_t& operator[](int index)
		{
			return Data[index];
		}

		uint8_t operator[](int index) const
		{
			return Data[index];
		}
		
	};
	*/
}