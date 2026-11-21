// When call api fail , we need to get error code and deep know why it is break! 
// On handle our error code saved and get with api GetLastError , but on Registry our error code will save on LSTATUS ! 

//example use :

// LSTATUS BaChaHom = ::RegQueryValueExW(...); <-- fail 
// if ( BaChaHom != ERROR_SUCCESS ) {
//	WhatSysSayRegistry(L"RegQueryValueExW false" , BaChaHom);
//}




void WhatSysSayRegistry(LPCWSTR apiContext, LSTATUS status)
{
	LPWSTR lpBuffer = nullptr;
	DWORD afterFormat = ::FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		status,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&lpBuffer),
		0,
		nullptr
	);
	std::wcerr << L"\n\nWe have problem at " << apiContext;
	std::wcerr << L"\nError code (decimal): " << status << " (heximal) 0x" << std::hex << status << std::dec;
	if (afterFormat > 0 || lpBuffer != nullptr)
	{
		std::wcerr << L"\nSystem say : " << lpBuffer;

	}
	if(lpBuffer == nullptr) {
		std::wcerr << L"\nSorry but system unknow what is it :P ";
	}
	else {
		::LocalFree(lpBuffer);
	}

	system("pause");

}
