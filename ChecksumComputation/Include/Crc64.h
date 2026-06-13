#pragma once

#include <stdint.h>
#include <algorithm>

namespace ChecksumComputation
{
	class Crc64
	{
	public:
		Crc64(uint64_t crcDivisor = 0) noexcept;

		void Initialize(uint64_t crcDivisor) noexcept;
		void ProcessData(void* data, size_t dataSize) noexcept;
		void Finalize() noexcept;

		uint64_t GetCrc64() const noexcept;

	private:
		uint64_t _crc64;
		uint64_t _crcDivisor;
		size_t _bytesRead;

		void ProcessByte(unsigned char nextReadByte) noexcept;

		bool CheckTopCrc64BitSet() const noexcept;
		bool CheckBitSet(unsigned char value, uint64_t bitNumber) const noexcept;
	};
}