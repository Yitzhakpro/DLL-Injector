#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include "utils/processUtils.h"
#include "utils/files.hpp"
#include "utils/prettyLogs.h"
#include <wil/resource.h>

int main(int argc, char* argv[])
{
	wchar_t processName[MAX_PATH] = { 0 };
	char dllPath[MAX_PATH] = { 0 };
	
	// getting name of process
	pretty_logs::question("Enter the name of the process (including .exe): ");
	std::wcin >> std::setw(MAX_PATH)  >> processName;
	int processPID = getProcId(processName);

	if (processPID == 0)
	{
		pretty_logs::error("Failed to find process id");
		return 1;
	}
	else
	{
		std::string process_found_msg = "Found process Id: " + std::to_string(processPID);
		pretty_logs::success(process_found_msg.c_str());
	}
	
	// getting the path of the dll
	pretty_logs::question("Enter the path of the dll: ");
	std::cin >> std::setw(MAX_PATH) >> dllPath;

	// checking of dll exists
	if (!fileExists(dllPath))
	{
		pretty_logs::error("Failed to find dll file");
		return 1;
	}

	// open process
	wil::unique_event hProc(OpenProcess(PROCESS_ALL_ACCESS, FALSE, processPID));

	if (hProc.get() == NULL)
	{
		pretty_logs::error("Fail to open target process!");
		return 1;
	}
	pretty_logs::info("Opening Target Process...");

	// allocating mem for dll
	LPVOID dllMemAlloc = VirtualAllocEx(hProc.get(), NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (dllMemAlloc == NULL)
	{
		pretty_logs::error("Fail to allocate memory in Target Process.");
		return 1;
	}
	pretty_logs::info("Allocating memory in Target Process.");

	// writing dll to process mem
	int isWriteOK = WriteProcessMemory(hProc.get(), dllMemAlloc, dllPath, strlen(dllPath) + 1, 0);
	if (isWriteOK == 0)
	{
		pretty_logs::error("Fail to write in Target Process memory.");
		return 1;
	}

	// creating remote thread in target process 
	pretty_logs::info("Creating Remote Thread in Target Process");
	LPTHREAD_START_ROUTINE addrLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(LoadLibrary(L"kernel32"), "LoadLibraryA");
	wil::unique_event ThreadReturn(CreateRemoteThread(hProc.get(), NULL, 0, addrLoadLibrary, dllMemAlloc, 0, NULL));
	if (ThreadReturn.get() == NULL)
	{
		pretty_logs::error("Fail to create Remote Thread");
		return 1;
	}

	if ((hProc.get() != NULL) && (dllMemAlloc != NULL) && (isWriteOK != ERROR_INVALID_HANDLE) && (ThreadReturn != NULL))
	{
		pretty_logs::success("DLL Successfully Injected.");

		WaitForSingleObject(ThreadReturn.get(), INFINITE);

		VirtualFreeEx(hProc.get(), dllMemAlloc, strlen(dllPath) + 1, MEM_RELEASE);
	}

	return 0;
}