// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2016, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_MUTEX_HPP_
#define POSEIDON_MUTEX_HPP_

#include "cxx_ver.hpp"
#include "cxx_util.hpp"
#include <boost/scoped_ptr.hpp>

namespace Poseidon {

class ConditionVariable;

class Mutex : NONCOPYABLE {
	friend ConditionVariable;

private:
	class Impl; // pthread_mutex_t

public:
	class UniqueLock : NONCOPYABLE {
		friend ConditionVariable;

	private:
		Mutex *m_owner;
		bool m_locked;

	public:
		UniqueLock();
		explicit UniqueLock(Mutex &owner, bool locks_owner = true);
		~UniqueLock();

#ifdef POSEIDON_CXX11
		UniqueLock(const UniqueLock &rhs) = delete;
		UniqueLock &operator=(const UniqueLock &rhs) = delete;

		UniqueLock(UniqueLock &&rhs) noexcept;
		UniqueLock &operator=(UniqueLock &&rhs) noexcept;
#else
		UniqueLock(const UniqueLock &)
			__attribute__((__error__("Use explicit STD_MOVE() to transfer ownership of unique locks.")));
		UniqueLock &operator=(const UniqueLock &)
			__attribute__((__error__("Use explicit STD_MOVE() to transfer ownership of unique locks.")));
#endif

	public:
		bool is_locked() const NOEXCEPT;
		void lock() NOEXCEPT;
		void unlock() NOEXCEPT;

		void swap(UniqueLock &rhs) NOEXCEPT;

	public:
#ifdef POSEIDON_CXX11
		explicit operator bool() const noexcept {
			return is_locked();
		}
#else
		typedef bool (UniqueLock::*DummyBool_)() const;
		operator DummyBool_() const NOEXCEPT {
			return is_locked() ? &UniqueLock::is_locked : 0;
		}
#endif
	};

private:
	const boost::scoped_ptr<Impl> m_impl;

public:
	explicit Mutex(bool recursive = false);
	~Mutex();
};

inline void swap(Mutex::UniqueLock &lhs, Mutex::UniqueLock &rhs) NOEXCEPT {
	lhs.swap(rhs);
}

}

#endif
