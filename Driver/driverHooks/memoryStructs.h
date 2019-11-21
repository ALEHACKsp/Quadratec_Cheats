#pragma once
#include "driverHooks.h"

typedef struct _COPY_MEMORY {
	void*  bufferAddress;         // Buffer address   
	UINT_PTR  address;        // Target address
	ULONGLONG  size;             // Buffer size
	ULONG     pid;              // Target process id
	BOOLEAN   write;            // TRUE if write operation, FALSE if read
	BOOLEAN  ReqBase;           // TRUE if request base address, FALSE if not.
	ULONG64 BaseAddress;    // Base address of the game
	void* Output;
	BOOLEAN ClearPIDCache;
	BOOLEAN PIDCacheCleared;
	BOOLEAN Read;
	BOOLEAN ReadString;
	BOOLEAN WriteString;
	const char* moduleName;
	ULONG pid_ofSource;

	// IChooseYou Added
	//UINT_PTR	process_id; already here 
	//PVOID		address; already here
	//SIZE_T		size;  already here

	BOOLEAN		change_protection;
	ULONG		protection;
	ULONG		protection_old;
}COPY_MEMORY;