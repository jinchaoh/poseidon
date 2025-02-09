// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2016, LH_Mouse. All wrongs reserved.

#include "../precompiled.hpp"
#include "upgraded_session_base.hpp"
#include "session.hpp"

namespace Poseidon {

namespace Http {
	UpgradedSessionBase::UpgradedSessionBase(const boost::shared_ptr<Session> &parent)
		: m_parent(parent)
	{
	}
	UpgradedSessionBase::~UpgradedSessionBase(){
	}

	void UpgradedSessionBase::on_connect(){
	}
	void UpgradedSessionBase::on_read_hup() NOEXCEPT {
	}
	void UpgradedSessionBase::on_close(int err_code) NOEXCEPT {
		(void)err_code;
	}

	bool UpgradedSessionBase::send(StreamBuffer buffer){
		const AUTO(parent, get_parent());
		if(!parent){
			return false;
		}
		return parent->TcpSessionBase::send(STD_MOVE(buffer));
	}

	bool UpgradedSessionBase::has_been_shutdown_read() const NOEXCEPT {
		const AUTO(parent, get_parent());
		if(!parent){
			return true;
		}
		return parent->TcpSessionBase::has_been_shutdown_read();
	}
	bool UpgradedSessionBase::shutdown_read() NOEXCEPT {
		const AUTO(parent, get_parent());
		if(!parent){
			return false;
		}
		return parent->TcpSessionBase::shutdown_read();
	}
	bool UpgradedSessionBase::has_been_shutdown_write() const NOEXCEPT {
		const AUTO(parent, get_parent());
		if(!parent){
			return true;
		}
		return parent->TcpSessionBase::has_been_shutdown_write();
	}
	bool UpgradedSessionBase::shutdown_write() NOEXCEPT {
		const AUTO(parent, get_parent());
		if(!parent){
			return false;
		}
		return parent->TcpSessionBase::shutdown_write();
	}
	void UpgradedSessionBase::force_shutdown() NOEXCEPT {
		const AUTO(parent, get_parent());
		if(!parent){
			return;
		}
		parent->TcpSessionBase::force_shutdown();
	}

	void UpgradedSessionBase::set_timeout(boost::uint64_t timeout){
		const AUTO(parent, get_parent());
		if(!parent){
			return;
		}
		parent->TcpSessionBase::set_timeout(timeout);
	}
}

}
