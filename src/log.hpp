// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2016, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_LOG_HPP_
#define POSEIDON_LOG_HPP_

#include "cxx_ver.hpp"
#include "cxx_util.hpp"
#include <sstream>
#include <cstddef>
#include <boost/cstdint.hpp>

namespace Poseidon {

class Logger : NONCOPYABLE {
public:
	enum {
		SP_POSEIDON     = 0x0080,
		SP_MAJOR        = 0x0040,

		LV_FATAL        = 0x0001 | SP_MAJOR,
		LV_ERROR        = 0x0002 | SP_MAJOR,
		LV_WARNING      = 0x0004 | SP_MAJOR,
		LV_INFO         = 0x0008,
		LV_DEBUG        = 0x0010,
		LV_TRACE        = 0x0020,
	};

public:
	static boost::uint64_t get_mask() NOEXCEPT;
	static boost::uint64_t set_mask(boost::uint64_t to_disable, boost::uint64_t to_enable) NOEXCEPT;

	static const char *get_thread_tag() NOEXCEPT;
	static void set_thread_tag(const char *new_tag) NOEXCEPT;

private:
	const boost::uint64_t m_mask;
	const char *const m_file;
	const std::size_t m_line;

	std::stringstream m_stream;

public:
	Logger(boost::uint64_t mask, const char *file, std::size_t line) NOEXCEPT;
	~Logger() NOEXCEPT;

private:
	// operator<< 的 name lookup 拖慢编译速度。
	void put(bool val);
	void put(char val);
	void put(signed char val);
	void put(unsigned char val);
	void put(short val);
	void put(unsigned short val);
	void put(int val);
	void put(unsigned val);
	void put(long val);
	void put(unsigned long val);
	void put(long long val);
	void put(unsigned long long val);
	void put(const char *val);
	void put(const signed char *val);
	void put(const unsigned char *val);
	void put(const void *val);

	template<typename T>
	void put(const T &val){
		m_stream <<val;
	}

public:
	template<typename T>
	Logger &operator,(const T &val) NOEXCEPT {
		try {
			this->put(val);
		} catch(...){
		}
		return *this;
	}
};

}

#define LOG_MASK(mask_, ...)    \
	do {    \
		::boost::uint64_t test_ = (mask_); \
		if(test_ & ::Poseidon::Logger::SP_MAJOR){   \
			test_ &= 0x3F;  \
		}   \
		if(test_ & ~(::Poseidon::Logger::get_mask())){  \
			break;  \
		}   \
		static_cast<void>(::Poseidon::Logger(mask_, __FILE__, __LINE__), __VA_ARGS__);  \
	} while(false)

#define LOG_POSEIDON(level_, ...)   \
	LOG_MASK(::Poseidon::Logger::SP_POSEIDON | (level_), __VA_ARGS__)

#define LOG_POSEIDON_FATAL(...)     LOG_POSEIDON(::Poseidon::Logger::LV_FATAL,      __VA_ARGS__)
#define LOG_POSEIDON_ERROR(...)     LOG_POSEIDON(::Poseidon::Logger::LV_ERROR,      __VA_ARGS__)
#define LOG_POSEIDON_WARNING(...)   LOG_POSEIDON(::Poseidon::Logger::LV_WARNING,    __VA_ARGS__)
#define LOG_POSEIDON_INFO(...)      LOG_POSEIDON(::Poseidon::Logger::LV_INFO,       __VA_ARGS__)
#define LOG_POSEIDON_DEBUG(...)     LOG_POSEIDON(::Poseidon::Logger::LV_DEBUG,      __VA_ARGS__)
#define LOG_POSEIDON_TRACE(...)     LOG_POSEIDON(::Poseidon::Logger::LV_TRACE,      __VA_ARGS__)

#endif
