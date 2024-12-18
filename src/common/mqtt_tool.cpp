#include "mqtt_tool.h"

namespace common {

common::MqttInfo parse_mqttInfo(Json::Value& item) {
  common::MqttInfo mqtt_ifo;
  mqtt_ifo.host = item["host"].asString();
  mqtt_ifo.port = item["port"].asInt();
  mqtt_ifo.user = item["user"].asString();
  mqtt_ifo.password = item["password"].asString();
  mqtt_ifo.nodePubTopic = item["nodePubTopic"].asString();
  mqtt_ifo.nodeSubTopic = item["nodeSubTopic"].asString();
  mqtt_ifo.addDevice = item["addDevice"].asString();
  mqtt_ifo.addDeviceResult = item["addDeviceResult"].asString();
  mqtt_ifo.updateDeviceStatus = item["updateDeviceStatus"].asString();
  mqtt_ifo.playStart = item["playStart"].asString();
  mqtt_ifo.playResult = item["playResult"].asString();
  mqtt_ifo.ok = 0;
  return mqtt_ifo;
}

Json::Value export_mqttInfo(common::MqttInfo& mqtt_ifo) {
  Json::Value item;
  item["host"] = mqtt_ifo.host;
  item["port"] = mqtt_ifo.port;
  item["user"] = mqtt_ifo.user;
  item["password"] = mqtt_ifo.password;
  item["nodePubTopic"] = mqtt_ifo.nodePubTopic;
  item["nodeSubTopic"] = mqtt_ifo.nodeSubTopic;
  item["addDevice"] = mqtt_ifo.addDevice;
  item["addDeviceResult"] = mqtt_ifo.addDeviceResult;
  item["updateDeviceStatus"] = mqtt_ifo.updateDeviceStatus;
  item["playStart"] = mqtt_ifo.playStart;
  item["playResult"] = mqtt_ifo.playResult;
  return item;
}


void on_connect_pub(struct mosquitto *mosq, void *obj, int reason_code)
{
	printf("on_connect_pub: %s\n", mosquitto_connack_string(reason_code));
	if(reason_code != 0){
		mosquitto_disconnect(mosq);
	}
}

void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
	printf("Message with mid %d has been published.\n", mid);
}


void on_connect_sub(struct mosquitto *mosq, void *obj, int reason_code)
{
	int rc;
	/* Print out the connection result. mosquitto_connack_string() produces an
	 * appropriate string for MQTT v3.x clients, the equivalent for MQTT v5.0
	 * clients is mosquitto_reason_string().
	 */
	printf("on_connect_sub: %s\n", mosquitto_connack_string(reason_code));
	if(reason_code != 0){
		/* If the connection fails for any reason, we don't want to keep on
		 * retrying in this example, so disconnect. Without this, the client
		 * will attempt to reconnect. */
		mosquitto_disconnect(mosq);
	}

}


/* Callback called when the broker sends a SUBACK in response to a SUBSCRIBE. */
void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
	int i;
	bool have_subscription = false;

	/* In this example we only subscribe to a single topic at once, but a
	 * SUBSCRIBE can contain many topics at once, so this is one way to check
	 * them all. */
	for(i=0; i<qos_count; i++){
		printf("on_subscribe: %d:granted qos = %d\n", i, granted_qos[i]);
		if(granted_qos[i] <= 2){
			have_subscription = true;
		}
	}
	if(have_subscription == false){
		/* The broker rejected all of our subscriptions, we know we only sent
		 * the one SUBSCRIBE, so there is no point remaining connected. */
		fprintf(stderr, "Error: All subscriptions rejected.\n");
		mosquitto_disconnect(mosq);
	}
}

/* Callback called when the client receives a message. */
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	// 通过http post方式转发数据
	std::string ret_str, message = std::string((char *)msg->payload);
	std::cout << "message:" << message << std::endl;
  	MqttSub* sub = (MqttSub*)obj;
	std::string httpServer = "http://127.0.0.1:"+std::to_string(sub->port);
	std::string httpApi = sub->httpApi;
	int ret = post_data_base(ret_str, message, httpServer, httpApi, -1, 1, nullptr);
}


MqttPub::MqttPub(common::MqttCfgData cfg_info, std::string topic):
    mosq(nullptr), cfg_ifo(cfg_info), topic(topic) {

}

MqttPub::~MqttPub() {
	mosquitto_lib_cleanup();
}

int MqttPub::init() {
    int rc = 0;
	mosquitto_lib_init();
	mosq = mosquitto_new(nullptr, true, this);
	if(mosq == nullptr){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}

	mosquitto_username_pw_set(mosq, cfg_ifo.user.c_str(), cfg_ifo.password.c_str());
	mosquitto_connect_callback_set(mosq, on_connect_pub);
	mosquitto_publish_callback_set(mosq, on_publish);

	rc = mosquitto_connect(mosq, cfg_ifo.host.c_str(), cfg_ifo.port, 60);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return 1;
	}

	/* Run the network loop in a background thread, this call returns quickly. */
	rc = mosquitto_loop_start(mosq);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return 1;
	}
	return 0;
}


void MqttPub::publish_data(std::string data)
{
	int rc;
	rc = mosquitto_publish(mosq, NULL, topic.c_str(), data.length(), data.c_str(), 2, false);
	if(rc != MOSQ_ERR_SUCCESS){
		fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
	}
}



MqttSub::MqttSub(common::MqttCfgData cfg_info, std::string topic, int port, std::string httpApi):
    mosq(nullptr), cfg_ifo(cfg_info), topic(topic), port(port), httpApi(httpApi) {

}

MqttSub::~MqttSub() {
	mosquitto_lib_cleanup();
}

int MqttSub::init() {
	int rc = 0;
	mosquitto_lib_init();
	mosq = mosquitto_new(NULL, true, this);
	if(mosq == NULL){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}


	mosquitto_username_pw_set(mosq, cfg_ifo.user.c_str(), cfg_ifo.password.c_str());
	mosquitto_connect_callback_set(mosq, on_connect_sub);
	mosquitto_subscribe_callback_set(mosq, on_subscribe);
	mosquitto_message_callback_set(mosq, on_message);

	rc = mosquitto_connect(mosq, cfg_ifo.host.c_str(), cfg_ifo.port, 60);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return 1;
	}
	
	rc = mosquitto_subscribe(mosq, NULL, topic.c_str(), 1);
	if(rc != MOSQ_ERR_SUCCESS){
		fprintf(stderr, "Error subscribing: %s\n", mosquitto_strerror(rc));
		/* We might as well disconnect if we were unable to subscribe */
		mosquitto_disconnect(mosq);
		return 1;
	}

	rc = mosquitto_loop_start(mosq);
	
	// std::thread(mosquitto_loop_forever, mosq, -1, 1).detach();
	return 0;
}


}