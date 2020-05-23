#include "proc.h"


DWORD get_proc_id(const wchar_t* proc_name)
{
	DWORD proc_id = 0;
	auto* const h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if (h_snap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 proc_entry;
		proc_entry.dwSize = sizeof(proc_entry);

		if (Process32First(h_snap, &proc_entry))
		{
			do
			{
				if(!_wcsicmp(proc_entry.szExeFile, proc_name))
				{
					proc_id = proc_entry.th32ProcessID;
					break;
				}
				
			}while(Process32Next(h_snap, &proc_entry));
		}
	}
	CloseHandle(h_snap);
	return proc_id;
}

uintptr_t get_module_base_address(const DWORD proc_id, const wchar_t* mod_name)
{
	uintptr_t mod_base_address = 0;
	auto* const h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);
	
	if(h_snap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 mod_entry;
		mod_entry.dwSize = sizeof(mod_entry);

		if(Module32First(h_snap, &mod_entry))
		{
			do
			{
				if(!_wcsicmp(mod_entry.szModule, mod_name))
				{
					mod_base_address = reinterpret_cast<uintptr_t>(mod_entry.modBaseAddr);
					break;
				}
				
			}while(Module32Next(h_snap, &mod_entry));
		}
	}
	CloseHandle(h_snap);
	return mod_base_address;
	
}

uintptr_t find_dma_addy(const HANDLE h_proc, const uintptr_t ptr, const std::vector<unsigned int>& offsets)
{
	auto address = ptr;
	
	for (auto offset : offsets)
	{
		ReadProcessMemory(h_proc, reinterpret_cast<BYTE*>(address), &address, sizeof(address), nullptr);
		address += offset;
	}

	return address;
}
