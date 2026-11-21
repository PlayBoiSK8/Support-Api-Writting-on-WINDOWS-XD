#include <windows.h>
#include <iostream>


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






/* example : 
int main() 
{
	size_t sizeReserve = 1024 * 1024; //1mb
	SmartVirtualMemory bigVirtual(::VirtualAlloc(nullptr,sizeReserve,MEM_RESERVE,PAGE_READWRITE));

}
*/