// HackAnyGameP2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "proc.h"

int main()
{
    //Get ProcId of the target process
    auto const proc_id = get_proc_id(L"ac_client.exe");

	//Get module base address
    const auto  module_base = get_module_base_address(proc_id, L"ac_client.exe");
	
	//Get handle to process
	HANDLE h_process = nullptr;
	h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, proc_id);

	//Resolve base address of the pointer chain
    const auto dynamic_ptr_base_address = module_base + 0x10f4f4;

    std::cout << "dynamic_ptr_base_address = " << "0x" << std::hex << dynamic_ptr_base_address << std::endl;
	
	//Resolve our ammo pointer chain
    const std::vector<unsigned int> ammo_offsets = {0x374, 0x14, 0x0};
    const auto ammo_address = find_dma_addy(h_process, dynamic_ptr_base_address, ammo_offsets);

	std::cout << "ammo_address = " << "0x" << std::hex << ammo_address << std::endl;
	
	//Read ammo value
	auto ammo_value = 0;

	ReadProcessMemory(h_process, reinterpret_cast<BYTE*>(ammo_address), &ammo_value, sizeof(ammo_value), nullptr);
    std::cout << "Current ammo = " << std::dec << ammo_value << std::endl;
	
	//Write to ammo value
	auto new_ammo = 1337;
	WriteProcessMemory(h_process, reinterpret_cast<BYTE*>(ammo_address), &new_ammo, sizeof(new_ammo), nullptr);
	
	//Read out again for confirmation
	ReadProcessMemory(h_process, reinterpret_cast<BYTE*>(ammo_address), &ammo_value, sizeof(ammo_value), nullptr);

    std::cout << "New ammo = " << std::dec << ammo_value << std::endl;

	getchar();
	return 0;
	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
