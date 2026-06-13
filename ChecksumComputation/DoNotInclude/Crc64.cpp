#include "pch.h"
#include "../Include/Crc64.h"

namespace ChecksumComputation
{
	Crc64::Crc64(uint64_t crcDivisor) noexcept : _crc64(0), _crcDivisor(crcDivisor), _bytesRead(0)
	{
	}

	void Crc64::Initialize(uint64_t crcDivisor) noexcept
	{
		_crc64 = 0;
		_crcDivisor = crcDivisor;
		_bytesRead = 0;
	}

	void Crc64::ProcessData(void* data, size_t dataSize) noexcept
	{
		if (data == nullptr || dataSize == 0)
			return;

		unsigned char* dataPointer = reinterpret_cast<unsigned char*>(data);

		uint64_t bytesToDirectlyCopy = 0;
		if (_bytesRead < 8)
		{
			uint64_t spacesLeft = 8 - _bytesRead;
			bytesToDirectlyCopy = std::min(spacesLeft, dataSize);

			for (size_t i = 0; i < bytesToDirectlyCopy; ++i)
			{
				_crc64 = _crc64 << 8;
				_crc64 += dataPointer[i];
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
		uint64_t bytesLeft = std::min(_bytesRead, 8ULL);

		for (size_t i = 0; i < bytesLeft; ++i)
		{
			for (size_t j = 0; j < 8; ++j)
			{
				bool bitSet = CheckTopCrc64BitSet();
				_crc64 = _crc64 << 1;
				if (bitSet)
					_crc64 = _crc64 ^ _crcDivisor;
			}
		}
	}

	uint64_t Crc64::GetCrc64() const noexcept
	{
		return _crc64;
	}

	void Crc64::ProcessByte(unsigned char nextReadByte) noexcept
	{
		for (size_t i = 0; i < 8; ++i)
		{
			bool crcBitSet = CheckTopCrc64BitSet();
			_crc64 = _crc64 << 1;
			if (CheckBitSet(nextReadByte, 7 - i))
				_crc64++;
			if (crcBitSet)
				_crc64 = _crc64 ^ _crcDivisor;
		}
	}

	bool Crc64::CheckTopCrc64BitSet() const noexcept
	{
		uint64_t bitValue = _crc64 & (1ULL << 63);
		return bitValue > 0;
	}

	bool Crc64::CheckBitSet(unsigned char value, uint64_t bitNumber) const noexcept
	{
		unsigned char bitValue = value & (1U << bitNumber);
		return bitValue > 0;
	}

}