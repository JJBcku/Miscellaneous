#pragma once

#include <stdint.h>
#include <algorithm>

namespace ChecksumComputation
{
	class Crc64
	{
	public:
		Crc64(uint64_t crcDivisor = 0);

		void Initialize(uint64_t divisor);
		void ProcessData(void* data, size_t dataSize) noexcept;
		void Finalize() noexcept;

	private:
		uint64_t crc64;
		uint64_t crcDivisor;
		uint64_t bytesRead;

		void ProcessByte(unsigned char nextReadByte) noexcept;

		bool CheckBitSet(uint64_t value, uint64_t bitNumber) const noexcept;
		bool CheckBitSet(unsigned char value, uint64_t bitNumber) const noexcept;
	};
}