// example use : 

// SmartRegKey OngChaHui;
// LSATUS status = ::RegOpenKeyExW(HKEY_CURRENT_USER , subkey , nullptr , KEY_QUERY_VALUE , OngChaHui.GetAddressOf()); 


class SmartRegKey {
public:
	explicit SmartRegKey(HKEY h = nullptr) noexcept :m_hkey(h) {}
	~SmartRegKey() noexcept{Close();}
	SmartRegKey(const SmartRegKey&) = delete;
	SmartRegKey& operator=(const SmartRegKey&) = delete;
	SmartRegKey(SmartRegKey&& other) noexcept :m_hkey(other.m_hkey) {
		other.m_hkey = nullptr;
	}

	SmartRegKey& operator=(SmartRegKey&& other) noexcept {
		if (this != &other) {
			m_hkey = other.m_hkey;
			other.m_hkey = nullptr;
		}return *this;
	}

	[[nodiscard]] HKEY Get() noexcept {
		return m_hkey;
	}

	PHKEY GetAddressOf() noexcept {
		Close();
		return &m_hkey;
	}

	[[nodiscard]] bool IsValid() const noexcept {
		return m_hkey != nullptr;
	}

	explicit operator bool() noexcept {
		return IsValid();
	}

	void Close() noexcept {
		if (IsValid()) {
			::RegCloseKey(m_hkey);
			m_hkey = nullptr;
		}
	}
private:
	HKEY m_hkey;
};
