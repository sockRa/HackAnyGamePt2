#pragma once

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>


DWORD get_proc_id(const wchar_t* proc_name);

uintptr_t get_module_base_address(DWORD proc_id, const wchar_t* mod_name);

uintptr_t find_dma_addy(HANDLE h_proc, uintptr_t ptr, const std::vector<unsigned int>& offsets);

