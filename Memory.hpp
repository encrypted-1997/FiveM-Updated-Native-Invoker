#pragma once

#include <Windows.h>
#include <iostream>

#include <vector>
#include <string>

#include <filesystem>
#include <fstream>

#include <mutex>
#include <map>
#include <unordered_map>


namespace Memory
{
	static uint64_t PatternScanEx(uint64_t pModuleBaseAddress, const char* sSignature, size_t nSelectResultIndex)
	{
		static auto patternToByte = [](const char* pattern)
			{
				auto       bytes = std::vector<int>{};
				const auto start = const_cast<char*>(pattern);
				const auto end = const_cast<char*>(pattern) + strlen(pattern);

				for (auto current = start; current < end; ++current)
				{
					if (*current == '?')
					{
						++current;
						if (*current == '?')
							++current;
						bytes.push_back(-1);
					}
					else
						bytes.push_back(strtoul((const char*)current, &current, 16));
				}
				return bytes;
			};

		const auto dosHeader = (PIMAGE_DOS_HEADER)pModuleBaseAddress;
		const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)pModuleBaseAddress + dosHeader->e_lfanew);

		const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto       patternBytes = patternToByte(sSignature);
		const auto scanBytes = reinterpret_cast<std::uint8_t*>(pModuleBaseAddress);

		const auto s = patternBytes.size();
		const auto d = patternBytes.data();

		size_t nFoundResults = 0;

		for (auto i = 0ul; i < sizeOfImage - s; ++i)
		{
			bool found = true;

			for (auto j = 0ul; j < s; ++j)
			{
				if (scanBytes[i + j] != d[j] && d[j] != -1)
				{
					found = false;
					break;
				}
			}

			if (found)
			{
				if (nSelectResultIndex != 0)
				{
					if (nFoundResults < nSelectResultIndex)
					{
						nFoundResults++;
						found = false;
					}
					else
						return reinterpret_cast<uint64_t>(&scanBytes[i]);
				}
				else
					return reinterpret_cast<uint64_t>(&scanBytes[i]);
			}
		}

		return NULL;
	}

	static uint64_t GetCurrentImageBase()
	{
		return *(uint64_t*)(__readgsqword(0x60) + 0x10);
	}

	static uint64_t ReslvReltveAdr(uint64_t Address, int InstructionLength)
	{
		if (!Address)
			return 0;

		DWORD Offset;
		if (!IsBadReadPtr((void*)(Address + (InstructionLength - 4)), sizeof(DWORD)))
		{
			Offset = *(DWORD*)(Address + (InstructionLength - 4));
		}
		else
		{
			return 0;
		}

		return Address + InstructionLength + Offset;
	}

	static uint64_t PatternScanNewEx(uint64_t pModuleBaseAdress, const char* sSignature, size_t nSelectResultIndex, int InstructionLength)
	{
		auto ret = PatternScanEx(pModuleBaseAdress, sSignature, nSelectResultIndex);

		if (!ret)
			return 0;

		DWORD Offset;
		if (!IsBadReadPtr((void*)(ret + (InstructionLength - 4)), sizeof(DWORD)))
		{
			Offset = *(DWORD*)(ret + (InstructionLength - 4));
		}
		else
		{
			return 0;
		}

		return ret + InstructionLength + Offset;
	}


	static uint64_t PatternScan(const char* sSignature, size_t nSelectResultIndex, int InstructionLength)
	{
		auto ret = PatternScanEx((uint64_t)GetModuleHandleA(nullptr), sSignature, nSelectResultIndex);

		if (InstructionLength != 0)
			ret = ReslvReltveAdr(ret, InstructionLength);

		//std::cout << E("\n [ ") << sSignature << E(" ] -> ") << E("0x") << std::hex << ret - (uint64_t)GetModuleHandleA(nullptr) << std::endl;

		return ret;
	}
}
