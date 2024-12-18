#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <mosquittopp.h>

#include "base_util/utils.h"
#include "common/base_struct.h"

namespace common {

common::MqttInfo parse_mqttInfo(Json::Value& item);
Json::Value export_mqttInfo(common::MqttInfo& mqtt_ifo);


void on_connect_pub(struct mosquitto *mosq, void *obj, int reason_code);

void on_publish(struct mosquitto *mosq, void *obj, int mid);

void on_connect_sub(struct mosquitto *mosq, void *obj, int reason_code);

void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos);

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);


class MqttPub
{
public:
    MqttPub(common::MqttCfgData cfg_info, std::string topic);
    ~MqttPub();
    int init();
    void publish_data(std::string data);

private:
    struct mosquitto *mosq;
    common::MqttCfgData cfg_ifo;
    std::string topic;
};


class MqttSub
{
public:
    MqttSub(common::MqttCfgData cfg_info, std::string topic, int port, std::string httpApi);
    ~MqttSub();
    int init();

public:
	int port;
	std::string httpApi;

private:
    struct mosquitto *mosq;
    common::MqttCfgData cfg_ifo;
    std::string topic;

};


}   // namespace smartbox
