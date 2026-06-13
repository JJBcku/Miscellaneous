#include "pch.h"
#include "Crc64.h"

namespace ChecksumComputation
{
	Crc64::Crc64(uint64_t crcDivisor) : crc64(0), crcDivisor(crcDivisor), bytesRead(0)
	{
	}

	void Crc64::Initialize(uint64_t divisor)
	{
		crc64 = 0;
		this->crcDivisor = divisor;
		bytesRead = 0;
	}

	void Crc64::ProcessData(void* data, size_t dataSize) noexcept
	{
		if (data == nullptr || dataSize == 0)
			return;

		unsigned char* dataPointer = reinterpret_cast<unsigned char*>(data);

		uint64_t bytesToDirectlyCopy = 0;
		if (bytesRead < 8)
		{
			uint64_t spacesLeft = 8 - bytesRead;
			bytesToDirectlyCopy = std::min(spacesLeft, dataSize);

			for (size_t i = 0; i < bytesToDirectlyCopy; ++i)
			{
				crc64 = crc64 << 8;
				crc64 += dataPointer[i];
			}
		}

		uint64_t bytesToProcess = dataSize - bytesToDirectlyCopy;
		unsigned char* processedDataPointer = &dataPointer[bytesToDirectlyCopy];
		for (size_t i = 0; i < bytesToProcess; ++i)
		{
			ProcessByte(*processedDataPointer);
			processedDataPointer++;
		}
	}

	void Crc64::Finalize() noexcept
	{
		uint64_t bytesLeft = std::min(bytesRead, 8ULL);

		for (size_t i = 0; i < bytesLeft; ++i)
		{
			for (size_t j = 0; j < 8; ++j)
			{
				bool bitSet = CheckBitSet(crc64, 63);
				crc64 = crc64 << 1;
				if (bitSet)
					crc64 = crc64 ^ crcDivisor;
			}
		}
	}

	void Crc64::ProcessByte(unsigned char nextReadByte) noexcept
	{
		for (size_t i = 0; i < 8; ++i)
		{
			bool crcBitSet = CheckBitSet(crc64, 63);
			crc64 = crc64 << 1;
			if (CheckBitSet(nextReadByte, 7 - i))
				crc64++;
			if (crcBitSet)
				crc64 = crc64 ^ crcDivisor;
		}
	}

	bool Crc64::CheckBitSet(uint64_t value, uint64_t bitNumber) const noexcept
	{
		uint64_t bitValue = value & (1ULL << bitNumber);
		return bitValue > 0;
	}

	bool Crc64::CheckBitSet(unsigned char value, uint64_t bitNumber) const noexcept
	{
		unsigned char bitValue = value & (1U << bitNumber);
		return bitValue > 0;
	}

}