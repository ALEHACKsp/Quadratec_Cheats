#pragma once
#include "driverHooks.h"
#include "functionsDef.h"
#include "NativeStructs.h"




// function def
// namespace

namespace functions {
	namespace DriverHooks {
		PVOID get_system_module_base(const char* module_name);
		PVOID get_system_module_export(const char* module_name,LPCSTR routine_name);
		bool ReadMemory(void* address, void* buffer, size_t size);
		bool WriteMemory(void* address, void* buffer, size_t size);
		bool WriteToReadOnlyMemory(void* address, void* buffer, size_t size);
		bool CallKernelFunction(void* kernel_function_address);
		bool WriteKernelMemory(HANDLE pid, uintptr_t address, void* buffer, SIZE_T size);
		NTSTATUS protect_virtual_memory( ULONG64 pid, PVOID address, ULONG size, ULONG protection, ULONG& protection_out );
		bool ReadKernelMemory(HANDLE pid, uintptr_t address, void* buffer, SIZE_T size);
		ULONG64 GetModuleBasex64(PEPROCESS proc, UNICODE_STRING module_name);
	}
}
