// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2016, LH_Mouse. All wrongs reserved.

#include "../precompiled.hpp"
#include "exception.hpp"
#include "../log.hpp"

namespace Poseidon {

namespace Cbpp {
	Exception::Exception(const char *file, std::size_t line, StatusCode status_code, SharedNts message)
		: ProtocolException(file, line, message, static_cast<long>(status_code))
	{
		LOG_POSEIDON(Logger::SP_MAJOR | Logger::LV_INFO,
			"Cbpp::Exception: status_code = ", status_code, ", what = ", what());
	}
	Exception::~Exception() NOEXCEPT {
	}
}

}
