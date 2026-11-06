#pragma once

#include <stdint.h>

#include <string>

#include "esp_err.h"

struct IdfSmsStatus {
    uint32_t total = 0;
    uint32_t lastSmsEpoch = 0;
    bool receiveReady = false;
};

esp_err_t idf_sms_start(void);
esp_err_t idf_sms_send_text(const std::string& phone, const std::string& text, std::string& message, bool track_reply = false);
esp_err_t idf_sms_enqueue_outgoing(const std::string& phone, const std::string& text, std::string& message);
int idf_sms_outgoing_queue_depth(void);
IdfSmsStatus idf_sms_get_status(void);

// 请求清空模组/SIM 侧短信存储。实际 AT 由短信任务在安全时机执行——Web 任务直接发
// CMGD 会和周期性 CMGL 轮询抢 AT 通道，还可能删掉正在解析的索引。返回 false 表示
// 短信任务尚未启动，请求未受理；eSIM 逻辑通道期间会延后到通道释放后执行。
bool idf_sms_request_purge_storage(void);

// 自定义短信任务体检：仅一个蜂窝任务可持有窗口；超时使用单调时钟。
bool idf_sms_reply_check_begin(const std::string& sender_pattern, const std::string& body_pattern);
void idf_sms_reply_check_arm(int timeout_seconds);
// 0=等待，1=匹配成功，-1=超时/未启用。
int idf_sms_reply_check_poll(void);
void idf_sms_reply_check_cancel(void);
