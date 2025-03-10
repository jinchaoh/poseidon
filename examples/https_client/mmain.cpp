#include <poseidon/precompiled.hpp>
#include <poseidon/log.hpp>
#include <poseidon/module_raii.hpp>
#include <poseidon/sock_addr.hpp>
#include <poseidon/async_job.hpp>
#include <poseidon/stream_buffer.hpp>
#include <poseidon/optional_map.hpp>
#include <poseidon/http/client.hpp>
#include <poseidon/singletons/epoll_daemon.hpp>
#include <poseidon/job_promise.hpp>
#include <poseidon/singletons/dns_daemon.hpp>
#include <poseidon/errno.hpp>
#include <poseidon/singletons/job_dispatcher.hpp>

// 客户端配置。
const char          g_host    [] = "github.com";
const unsigned      g_port       = 443;
const char          g_uri     [] = "/";

class Client : public Poseidon::Http::Client {
public:
	explicit Client(const SockAddr &addr)
		: Poseidon::Http::Client(addr, true) // use_ssl = true
	{
	}
	~Client(){
	}

protected:
	void on_sync_connect() override {
		LOG_POSEIDON_INFO("Connection established: remote = ", get_remote_info());
	}

	void on_sync_response_headers(Poseidon::Http::ResponseHeaders response_headers,
		std::string transfer_encoding, boost::uint64_t content_length) override
	{
		LOG_POSEIDON_INFO("Response: HTTP version = ", response_headers.version / 10000, ".", response_headers.version % 10000);
		LOG_POSEIDON_INFO("Response: Status code = ", response_headers.status_code);
		LOG_POSEIDON_INFO("Response: Reason = ", response_headers.reason);
		for(auto it = response_headers.headers.begin(); it != response_headers.headers.end(); ++it){
			LOG_POSEIDON_INFO("Response header: ", it->first, ": ", it->second);
		}

		LOG_POSEIDON_WARNING("Transfer-Encoding : ", transfer_encoding);
		LOG_POSEIDON_WARNING("Content-Length    : ", content_length);
	}
	void on_sync_response_entity(boost::uint64_t entity_offset, bool is_chunked, Poseidon::StreamBuffer entity) override {
		LOG_POSEIDON_DEBUG("Recevied entity: entity_offset = ", entity_offset, ", is_chunked = ", is_chunked, ", entity = ", entity.dump());
	}
	void on_sync_response_end(boost::uint64_t content_length, bool is_chunked, Poseidon::OptionalMap headers) override {
		LOG_POSEIDON_WARNING("End of entity: content_length = ", content_length, ", is_chunked = ", is_chunked);

		for(auto it = headers.begin(); it != headers.end(); ++it){
			LOG_POSEIDON_INFO("Additional chunked header: ", it->first, ": ", it->second);
		}
	}
};

MODULE_RAII(/* handles */){
	Poseidon::enqueue_async_job(
		[]{
			const auto sock_addr = boost::make_shared<Poseidon::SockAddr>();
			LOG_POSEIDON_FATAL("Looking up host: ", g_host);
			{
				const auto promise = Poseidon::DnsDaemon::enqueue_for_looking_up(sock_addr, g_host, g_port);
				Poseidon::JobDispatcher::yield(promise);
				promise->check_and_rethrow();
			}

			LOG_POSEIDON_FATAL("Creating client to ", Poseidon::get_ip_port_from_sock_addr(*sock_addr));
			const auto client = boost::make_shared<Client>(*sock_addr);
			client->go_resident();

			LOG_POSEIDON_FATAL("Creating HTTP request for ", g_uri);
			Poseidon::Http::RequestHeaders request_headers;
			request_headers.verb     = Poseidon::Http::V_GET;
			request_headers.uri      = g_uri;
			request_headers.version  = 10001;
			request_headers.headers.set(Poseidon::sslit("Host"), g_host);
			request_headers.headers.set(Poseidon::sslit("Connection"), "Close");
			client->send(std::move(request_headers));
		});
}
