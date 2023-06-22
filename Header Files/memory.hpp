#pragma once

#include "common.h"
#include "../Valve/SDK/Vector3.h"
#include <Psapi.h>

class c_memory
{
	HANDLE m_handle = nullptr;
	std::string m_name;
	PROCESSENTRY32 m_data;


public:

	c_memory() { }

	c_memory(const std::string_view process_name) : m_name(process_name)
	{

	}

	bool attach()
	{
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 entry = { sizeof(PROCESSENTRY32) };

		if (Process32First(snapshot, &entry) == TRUE)
		{
			do
			{
				if (_stricmp(entry.szExeFile, m_name.data()) == 0)
				{
					m_handle = OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID);
					m_data = entry;
					break;
				}
			} while (Process32Next(snapshot, &entry) == TRUE);
		}

		CloseHandle(snapshot);

		return m_handle != nullptr;
	}

	void close()
	{
		CloseHandle(m_handle);
	}

	template<typename type = std::uintptr_t>
	bool is_valid(const std::uintptr_t address)
	{
		return address > 0x400000 &&
			(address + sizeof(type) < 0x00007FFFFFFF0000) ||
			(address % 2) != 0;
	}

	template<typename type>
	type read(const std::uintptr_t address)
	{
		type buf = {};

		if (!is_valid<type>(address))
			return buf;

		ReadProcessMemory(m_handle, reinterpret_cast<void*>(address), &buf, sizeof(buf), nullptr);

		return buf;
	}

	template<typename type>
	void write(const std::uintptr_t address, type value)
	{
		WriteProcessMemory(m_handle, reinterpret_cast<void*>(address), &value, sizeof(value), nullptr);
	}

	static DWORD FindPattern(const char* module_name, const BYTE* mask, const char* mask_string)
	{
		/// Get module address
		const unsigned int module_address = reinterpret_cast<unsigned int>(GetModuleHandle(module_name));

		/// Get module information to the size
		MODULEINFO module_info;
		GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(module_address), &module_info, sizeof(MODULEINFO));

		auto IsCorrectMask = [](const unsigned char* data, const unsigned char* mask, const char* mask_string) -> bool
		{
			for (; *mask_string; ++mask_string, ++mask, ++data)
				if (*mask_string == 'x' && *mask != *data)
					return false;

			return (*mask_string) == 0;
		};

		/// Iterate until we find a matching mask
		for (unsigned int c = 0; c < module_info.SizeOfImage; c += 1)
		{
			/// does it match?
			if (IsCorrectMask(reinterpret_cast<unsigned char*>(module_address + c), mask, mask_string))
				return (module_address + c);
		}

		return 0;
	}


	MODULEENTRY32 get_module(const std::string_view module_name)
	{
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_data.th32ProcessID);
		MODULEENTRY32 entry = { sizeof(MODULEENTRY32) };

		if (Module32First(snapshot, &entry) == TRUE)
		{
			do
			{
				if (_stricmp(entry.szModule, module_name.data()) == 0)
				{
					CloseHandle(snapshot);
					return entry;
				}
			} while (Module32Next(snapshot, &entry) == TRUE);

		}

		CloseHandle(snapshot);

		return {};
	}
	PROCESSENTRY32& data() { return m_data; }
};

extern c_memory mem;