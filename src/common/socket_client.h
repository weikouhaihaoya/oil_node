#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <map>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include "base_util/utils.h"
#include "algorithm/total_algo.h"
#include "led_control.h"

namespace common {

Json::Value get_device_info();


typedef websocketpp::client<websocketpp::config::asio_client> client;

class WebsocketEndpoint;

class ConnectionMetadata {
public:
	typedef std::shared_ptr<ConnectionMetadata> ptr;

	ConnectionMetadata(ConfigInfo* cfg_ifo, LogInfo* log_ifo, int id, websocketpp::connection_hdl hdl, std::string uri);

	void on_open(client * c, websocketpp::connection_hdl hdl, void* cli);

	void on_fail(client * c, websocketpp::connection_hdl hdl);
	
	void on_close(client * c, websocketpp::connection_hdl hdl);

	void on_message(websocketpp::connection_hdl, client::message_ptr msg, void* cli);

	websocketpp::connection_hdl get_hdl() const {
		return m_hdl;
	}
	
	int get_id() const {
		return m_id;
	}
	
	std::string get_status() const {
		return m_status;
	}

public:
	std::map<std::string, std::set<std::string>> listener;
	std::map<int, LEDControlData> led_status;
	std::map<int, std::string> id2ip;
	
	friend std::ostream & operator<< (std::ostream & out, ConnectionMetadata const & data);
	friend WebsocketEndpoint;
private:
	int m_id;
	websocketpp::connection_hdl m_hdl;
	std::string m_status;
	std::string m_uri;
	std::string m_server;
	std::string m_error_reason;

	ConfigInfo* cfg_ifo; 
	LogInfo* log_ifo;
};



class WebsocketEndpoint {
public:
	WebsocketEndpoint (ConfigInfo* cfg_ifo, LogInfo* log_ifo);

	~WebsocketEndpoint();

	int connect(std::string const & uri);

	void close(int id, websocketpp::close::status::value code, std::string reason);

	void send(int id, std::string message);

	void send_heart(int id);

	void bind_usr(int id);

	void send_common(int id, std::string type, std::string from_user, 
		std::string to_usr, std::string unique_code, int status_code, Json::Value msg);

	void send_vihicle_info(int id, std::string type, int64 cur_time, std::vector<BaseInfoPD*>& infos);

	ConnectionMetadata::ptr get_metadata(int id) const;

public:
	typedef std::map<int,ConnectionMetadata::ptr> con_list;
	con_list connection_list;

private:

	client m_endpoint;
	std::shared_ptr<std::thread> m_thread;

	int m_next_id;

	ConfigInfo* cfg_ifo; 
	LogInfo* log_ifo;
};

}   // namespace smartbox

