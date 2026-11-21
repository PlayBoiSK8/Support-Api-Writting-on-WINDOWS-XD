#include <windows.h>
#include <iostream>

/* Option 
class SmartVirtualMemory {
public:
	explicit SmartVirtualMemory(LPVOID baseAddress = nullptr) noexcept :m_baseAddress(baseAddress) {}
	~SmartVirtualMemory() noexcept { Release(); }
	SmartVirtualMemory(const SmartVirtualMemory&) = delete;
	SmartVirtualMemory& operator=(const SmartVirtualMemory&) = delete;
	SmartVirtualMemory(SmartVirtualMemory&& other) noexcept :m_baseAddress(other.m_baseAddress) {
		other.m_baseAddress = nullptr;
	}

	SmartVirtualMemory& operator=(SmartVirtualMemory&& other) noexcept {
		if (this != &other) {
			Release();
			m_baseAddress = other.m_baseAddress;
			other.m_baseAddress = nullptr;
		}return *this;
	}

	[[nodiscard]] LPVOID Get() noexcept {
		return m_baseAddress;
	}

	BYTE* AsByte() noexcept {
		return static_cast<BYTE*>(m_baseAddress);
	}

	[[nodiscard]] bool IsValid() const noexcept {
		return m_baseAddress != nullptr;
	}

	explicit operator bool() noexcept {
		return IsValid();
	}

	void Release() noexcept {
		if (IsValid()) {
			::VirtualFree(m_baseAddress, 0, MEM_RELEASE);
			m_baseAddress = nullptr;
		}
	}
private:
	LPVOID m_baseAddress;
};
*/



void WhatSysSaySehError(DWORD errorCode)
{
	LPWSTR saveLog = nullptr;
	HMODULE hNtDll = ::GetModuleHandleW(L"ntdll.dll");
	DWORD afterFormat = ::FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_HMODULE |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		hNtDll,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast <LPWSTR> (&saveLog),
		0,
		nullptr
	);

	std::wcerr << L"\n\nYour code SEH is 0x" << std::hex << errorCode;

	if (afterFormat > 0 && saveLog != nullptr) {
		std::wcerr << L"\nAnd System say : " << saveLog;
		::LocalFree(saveLog);
	}
	else {
		std::wcerr << L"\nSorry dude but system dont know what happen ! :D ";
	}
}


bool TryGetError(BYTE* target, BYTE payload )
{
	DWORD errorCode = 0;
	__try 
	{
		*target = payload;
		return true;
	}

	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		errorCode = ::GetExceptionCode();
		WhatSysSaySehError(errorCode);
		return false;
	}
}


/* example : 
int main() 
{
	size_t sizeReserve = 1024 * 1024; //1mb
	SmartVirtualMemory bigVirtual(::VirtualAlloc(nullptr,sizeReserve,MEM_RESERVE,PAGE_READWRITE));
	
	BYTE* page0 = bigVirtual.AsByte();
	
	if (!TryGetError(page0, 0x41))
	{
	
	}

}
*/