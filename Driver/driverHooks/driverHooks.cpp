#include "driverHooks.h"
#include "Memory.h"
#include "memoryStructs.h"
#include "ClearPIDcache.h"



NTSTATUS hook_handler(PVOID called_param)
{

	COPY_MEMORY* m = (COPY_MEMORY*)called_param;	

	// Added by IChooseYou

	if ( m->change_protection )
	{
		functions::DriverHooks::protect_virtual_memory( m->pid, (PVOID)m->address, m->size, m->protection, m->protection_old );
	}

	if (m->ReqBase != FALSE) {
		ANSI_STRING AS;
		UNICODE_STRING ModuleNAme;
		  
		RtlInitAnsiString(&AS, m->moduleName);
		RtlAnsiStringToUnicodeString(&ModuleNAme, &AS, TRUE);


		PEPROCESS process;
		PsLookupProcessByProcessId((HANDLE)m->pid, &process);
		ULONG64 Base_Address64 = NULL;
		Base_Address64 = functions::DriverHooks::GetModuleBasex64(process, ModuleNAme);
		m->BaseAddress = Base_Address64;
		RtlFreeUnicodeString(&ModuleNAme);
	}

	if (m->write != FALSE) {
		PVOID kernelBuff = ExAllocatePool(NonPagedPool, m->size);

		if (!kernelBuff) 
			return STATUS_UNSUCCESSFUL;
		
			
		if (!memcpy(kernelBuff, m->bufferAddress, m->size))
			return STATUS_UNSUCCESSFUL;
		
		PEPROCESS Process;
		PsLookupProcessByProcessId((HANDLE)m->pid, &Process);
		functions::DriverHooks::WriteKernelMemory((HANDLE)m->pid, m->address, kernelBuff, m->size);
		ExFreePool(kernelBuff);
	}


	if(m->Read != FALSE)
	{
		void* ReadOutput = NULL;
		functions::DriverHooks::ReadKernelMemory((HANDLE)m->pid, m->address, &ReadOutput, m->size);
		m->Output = ReadOutput;
	}

	if (m->ClearPIDCache != FALSE) {

		if (ClearPIDB() != FALSE) {
			m->PIDCacheCleared = TRUE;
		}
		else
		{
			m->PIDCacheCleared = FALSE;
		}
	}

	if (m->ReadString != FALSE) {
		// read it here.
		PVOID kernelBuffer = ExAllocatePool(NonPagedPool, m->size);

		if (!kernelBuffer)
			return STATUS_UNSUCCESSFUL;


		if (!memcpy(kernelBuffer, m->bufferAddress, m->size))
			return STATUS_UNSUCCESSFUL;

		functions::DriverHooks::ReadKernelMemory((HANDLE)m->pid, m->address, kernelBuffer, m->size);

		RtlZeroMemory(m->bufferAddress, m->size);

		if (!memcpy(m->bufferAddress, kernelBuffer, m->size))
			return STATUS_UNSUCCESSFUL;
		
		ExFreePool(kernelBuffer);
	}

	if (m->WriteString != FALSE) {
		PVOID kernelBuffer1 = ExAllocatePool(NonPagedPool, m->size);

		if (!kernelBuffer1)
			return STATUS_UNSUCCESSFUL;


		if (!memcpy(kernelBuffer1, m->bufferAddress, m->size))
			return STATUS_UNSUCCESSFUL;

		PEPROCESS Process;
		PsLookupProcessByProcessId((HANDLE)m->pid, &Process);
		functions::DriverHooks::WriteKernelMemory((HANDLE)m->pid, m->address, kernelBuffer1, m->size);

		ExFreePool(kernelBuffer1);
	}

	return STATUS_SUCCESS;
}



extern "C" NTSTATUS DriverEntry(
	PDRIVER_OBJECT  driver_object,
	PUNICODE_STRING registry_path)
{
	// These are invalid for mapped drivers.
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(registry_path);


	functions::DriverHooks::CallKernelFunction(&hook_handler);
	
	return STATUS_SUCCESS;
}



