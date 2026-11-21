//example use:

//SmartHandle amChaThom(::CreateFileW(...));
// if (!amChaThom){WhatSysSay(L"amChaThom")};



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
