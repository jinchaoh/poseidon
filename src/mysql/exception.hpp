// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2016, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_MYSQL_EXCEPTION_HPP_
#define POSEIDON_MYSQL_EXCEPTION_HPP_

#include "../protocol_exception.hpp"

namespace Poseidon {

namespace MySql {
	class Exception : public ProtocolException {
	public:
		Exception(const char *file, std::size_t line, long error_code, SharedNts message);
		~Exception() NOEXCEPT;
	};
}

}

#endif
