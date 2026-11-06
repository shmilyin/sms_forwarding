#pragma once

#include <stddef.h>
#include <stdint.h>

#include <string>

struct IdfInboxEntry {
    uint32_t id = 0;
    uint32_t recvEpoch = 0;
    std::string sender;
    std::string ts;
    std::string text;
    bool forwarded = false;
};

struct IdfSentEntry {
    uint32_t id = 0;
    uint32_t sentEpoch = 0;
    std::string target;
    std::string text;
    bool ok = false;
};

void idf_inbox_init(void);
uint32_t idf_inbox_add(const char* sender, const char* text, const char* ts);
void idf_inbox_mark_forwarded(uint32_t id);
// 投递最终失败时把条目改回"未转发"，让用户能在收件箱看到并手动重发
void idf_inbox_set_forwarded(uint32_t id, bool forwarded);
size_t idf_inbox_count(void);
bool idf_inbox_get_by_id(uint32_t id, IdfInboxEntry& out);
bool idf_inbox_delete(uint32_t id);
// 一键清空：内存环与 smsdata 分区里的对应条目一起抹掉，返回实际移除的条数。
// sent_box=true 清发件箱，false 清收件箱；两者互不影响。
size_t idf_inbox_clear(bool sent_box);

void idf_sent_add(const char* target, const char* text, bool ok);

std::string idf_inbox_json(bool sent_box, int limit);
