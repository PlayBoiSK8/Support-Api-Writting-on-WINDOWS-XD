//example use : 

// SmartHandle ahihiDoNgok(::CreateFileW(...));



class SmartHandle {
public:
	explicit SmartHandle(HANDLE h = INVALID_HANDLE_VALUE) noexcept:m_handle(h){}
	~SmartHandle() noexcept { Close(); }
	SmartHandle(const SmartHandle&) = delete;
	SmartHandle& operator=(const SmartHandle&) = delete;
	SmartHandle(SmartHandle&& other) noexcept :m_handle(other.m_handle) {
		other.m_handle = INVALID_HANDLE_VALUE;
	}
	SmartHandle& operator=(SmartHandle&& other)noexcept{
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

	void Close() noexcept{
		if (IsValid())
		{
			::CloseHandle(m_handle);
			m_handle = INVALID_HANDLE_VALUE;
		}
	}

private:
	HANDLE m_handle;
};

