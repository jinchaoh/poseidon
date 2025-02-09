// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2016, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_SYSTEM_EXCEPTION_HPP_
#define POSEIDON_SYSTEM_EXCEPTION_HPP_

#include "exception.hpp"
#include <cerrno>

namespace Poseidon {

class SystemException : public Exception {
private:
	int m_code;

public:
	SystemException(const char *file, std::size_t line, int code = errno);
	~SystemException() NOEXCEPT;

public:
	int code() const NOEXCEPT {
		return m_code;
	}
};

}

#endif
