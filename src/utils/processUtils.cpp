#include "processUtils.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <wil/resource.h>

int getProcId(const wchar_t* name_of_process)
{
	DWORD pID = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	wil::unique_event hSnapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));

	do
	{
		if (wcscmp(pe32.szExeFile, name_of_process) == 0)
		{
			pID = pe32.th32ProcessID;
			break;
		}

	} while (Process32Next(hSnapshot.get(), &pe32));

	return pID;
}