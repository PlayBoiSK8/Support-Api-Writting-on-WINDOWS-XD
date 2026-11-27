#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <psapi.h>




class SmartHandle {
public:
	explicit SmartHandle(HANDLE h = INVALID_HANDLE_VALUE) noexcept :m_handle(h) {}
	~SmartHandle() noexcept { Close(); }
	SmartHandle(const SmartHandle&) = delete;
	SmartHandle& operator=(const SmartHandle&) = delete;
	SmartHandle(SmartHandle&& other) noexcept :m_handle(other.m_handle) {
		other.m_handle = INVALID_HANDLE_VALUE;
	}
	SmartHandle& operator=(SmartHandle&& other)noexcept {
		if (this != &other) {
			Close();
			m_handle = other.m_handle;
			other.m_handle = INVALID_HANDLE_VALUE;
		}
	}

	[[nodiscard]] HANDLE Get() const noexcept {
		return m_handle;
	}

	HANDLE* GetAddressOf() noexcept {
		Close();
		return &m_handle;
	}

	[[nodiscard]] bool IsValid() noexcept {
		return m_handle != NULL && m_handle != INVALID_HANDLE_VALUE;
	}

	explicit operator bool() noexcept {
		return IsValid();
	}

	HANDLE Detach() noexcept {
		HANDLE temp = m_handle;
		m_handle = INVALID_HANDLE_VALUE;
		return temp;
	}

	void Attach(HANDLE h) noexcept {
		Close();
		m_handle = h;
	}

	void Close() noexcept {
		if (IsValid())
		{
			::CloseHandle(m_handle);
			m_handle = INVALID_HANDLE_VALUE;
		}
	}

private:
	HANDLE m_handle;
};

const char* GetStateString(DWORD state)
{
	switch (state)
	{
	case MEM_COMMIT: return "COMMIT ";
	case MEM_RESERVE: return "REVERSE ";
	case MEM_FREE: return "FREE ";
	default: return "UNKNOW ";
	}
}

const char* GetTypeString(DWORD type)
{
	switch (type)
	{
	case MEM_IMAGE: return "IMAGE ";
	case MEM_MAPPED: return "MAPPED ";
	case MEM_PRIVATE: return "PRIVATED ";
	default: return "N/A ";

	}
}

void PrintProtectString(DWORD protect)
{
	if (protect == 0)
	{
		printf("%-4s", "---");
		return;
	}
	DWORD baseProtect = protect & 0xFF;
	const char* perm = "OTHER";

	switch (baseProtect) {
	case PAGE_NOACCESS:          perm = "NOACCESS"; break;
	case PAGE_READONLY:          perm = "R--";      break;
	case PAGE_READWRITE:         perm = "RW-";      break;
	case PAGE_WRITECOPY:         perm = "WC-";      break;
	case PAGE_EXECUTE:           perm = "--X";      break;
	case PAGE_EXECUTE_READ:      perm = "R-X";      break;
	case PAGE_EXECUTE_READWRITE: perm = "RWX";      break;
	case PAGE_EXECUTE_WRITECOPY: perm = "WCX";      break;
	}
	if (protect & PAGE_GUARD)
	{
		printf("%-5s + GUARD ", perm);
	}
	else
	{
		printf("%-14s", perm);
	}

}

void WhatSysSay(LPCWSTR apiContext)
{
	DWORD getCodeError = ::GetLastError();
	LPWSTR saveLog = nullptr;

	DWORD afterFormat = ::FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		getCodeError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast <LPWSTR> (&saveLog),
		0,
		nullptr
	);

	std::wcerr << L"\nWe have problem at " << apiContext;
	std::wcerr << L"\nError code decimal is " << getCodeError;
	std::wcerr << L"\nError code heximal is " << std::hex << getCodeError << std::dec;

	if (afterFormat > 0 && saveLog != nullptr) {
		std::wcerr << L"\nAnd System say : " << saveLog;
	}
	else {
		std::wcerr << L"\nSorry dude but system dont know what happen ! :D ";
	}

	if (saveLog != nullptr) {
		::LocalFree(saveLog);
	}

}

bool IsExecutable(DWORD protect) {
	DWORD base = protect & 0xFF;
	return base == PAGE_EXECUTE ||
		base == PAGE_EXECUTE_READ ||
		base == PAGE_EXECUTE_READWRITE ||
		base == PAGE_EXECUTE_WRITECOPY;
}

int main(int argc, char* argv[])
{
	DWORD pid = 0;
	if (argc == 2)
	{
		pid = std::strtoul(argv[1], nullptr, 10);
	}
	else
	{
		std::wcout << L"\n\nU need to run on cmd for example like : MemoryMap.exe 666(pid) "
			<< L"\nMaybe u donk how to use for firt time , so i will scan in this process MemoryMap.exe for example Log :D ";
		pid = ::GetCurrentProcessId();
	}

	SmartHandle openProcess(::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid));
	if (!openProcess)
	{
		WhatSysSay(L"SmartVirtualMemory openProcess");
	}

	SYSTEM_INFO st = {};
	::GetSystemInfo(&st);
	LPBYTE baseAddress = (LPBYTE)st.lpMinimumApplicationAddress;
	LPBYTE maxAddress = (LPBYTE)st.lpMaximumApplicationAddress;


	std::wcout << L"\nScan virtual start: 0x" << baseAddress <<
		L" end: 0x" << maxAddress << L"\n";



	MEMORY_BASIC_INFORMATION mbi = {};
	while (baseAddress < maxAddress)
	{
		SIZE_T byteReturn = ::VirtualQueryEx(openProcess.Get(), baseAddress, &mbi, sizeof(mbi));
		if (byteReturn == 0)
		{
			break;
		}

		printf("\n0x%p | 0x%p | 0x%-10zX | %-10zs | %-10zs | ",
			mbi.BaseAddress,
			mbi.AllocationBase,
			mbi.RegionSize,
			GetStateString(mbi.State),
			GetTypeString(mbi.Type));

		PrintProtectString(mbi.Protect);

			WCHAR path[MAX_PATH] {};
			if (mbi.Type == MEM_MAPPED || mbi.Type == MEM_IMAGE)
			{
				if (::GetMappedFileNameW(openProcess.Get(), mbi.BaseAddress, path, MAX_PATH) > 0)
				{
					std::printf ("%ls ", path);
				}
			}
		

		LPBYTE nextAddr = (LPBYTE)mbi.BaseAddress + mbi.RegionSize;
		if (nextAddr <= baseAddress)
		{
			break;
		}
		baseAddress = nextAddr;
	}






	std::cout << "\n\n";
	system("pause");
}
