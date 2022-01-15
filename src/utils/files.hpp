#pragma once
#include <Windows.h>

bool fileExists(char* path_to_file)
{
	wchar_t file_path[MAX_PATH];
	size_t path_size = strlen(path_to_file) + 1;
	mbstowcs_s(&path_size, file_path, path_to_file, path_size);
	LPWSTR lpPath = file_path;

	DWORD dwAttrib = GetFileAttributes(lpPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}