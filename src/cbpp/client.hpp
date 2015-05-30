// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2015, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_CBPP_CLIENT_HPP_
#define POSEIDON_CBPP_CLIENT_HPP_

#include "../tcp_client_base.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include "control_codes.hpp"
#include "status_codes.hpp"

namespace Poseidon {

class TimerItem;

namespace Cbpp {
	class Client : public TcpClientBase, private Reader, private Writer {
	private:
		class SyncJobBase;
		class ConnectJob;
		class DataMessageHeaderJob;
		class DataMessagePayloadJob;
		class DataMessageEndJob;
		class ErrorMessageJob;

	private:
		static void keepAliveTimerProc(const boost::weak_ptr<Client> &weakClient, boost::uint64_t now, boost::uint64_t period);

	private:
		const boost::uint64_t m_keepAliveInterval;

		boost::shared_ptr<TimerItem> m_keepAliveTimer;
		boost::uint64_t m_lastPongTime;

	protected:
		Client(const SockAddr &addr, bool useSsl, boost::uint64_t keepAliveInterval);
		Client(const IpPort &addr, bool useSsl, boost::uint64_t keepAliveInterval);
		~Client();

	protected:
		// TcpSessionBase
		void onConnect() OVERRIDE;

		void onReadAvail(StreamBuffer data) OVERRIDE;

		// Reader
		void onDataMessageHeader(boost::uint16_t messageId, boost::uint64_t payloadSize) OVERRIDE;
		void onDataMessagePayload(boost::uint64_t payloadOffset, StreamBuffer payload) OVERRIDE;
		bool onDataMessageEnd(boost::uint64_t payloadSize) OVERRIDE;

		bool onControlMessage(ControlCode controlCode, boost::int64_t vintParam, std::string stringParam) OVERRIDE;

		// Writer
		long onEncodedDataAvail(StreamBuffer encoded) OVERRIDE;

		// 可覆写。
		virtual void onSyncConnect();

		virtual void onSyncDataMessageHeader(boost::uint16_t messageId, boost::uint64_t payloadSize) = 0;
		virtual void onSyncDataMessagePayload(boost::uint64_t payloadOffset, const StreamBuffer &payload) = 0;
		virtual void onSyncDataMessageEnd(boost::uint64_t payloadSize) = 0;

		virtual void onSyncErrorMessage(boost::uint16_t messageId, StatusCode statusCode, const std::string &reason);

	public:
		bool send(boost::uint16_t messageId, StreamBuffer payload);
		bool sendControl(ControlCode controlCode, boost::int64_t vintParam, std::string stringParam);

		template<typename MsgT>
		bool send(const MsgT &msg){
			return send(MsgT::ID, StreamBuffer(msg));
		}
	};
}

}

#endif
