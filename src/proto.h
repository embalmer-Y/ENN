#ifndef __PROTO_H__
#define __PROTO_H__

#include <stdint.h>
#include <time.h>

#define MSG_PRIORITY_BIT_MASK 0x7

enum msg_priority {
    MSG_PRIO_LOW = 0,
    MSG_PRIO_LEVEL1,
    MSG_PRIO_LEVEL2,
    MSG_PRIO_LEVEL3,
    MSG_PRIO_LEVEL4,
};

/*
 protocol block schematic:
 32 bits                                   16 bits                                  0 bits
 +-------8bits--------+-------8bits--------+-------8bits-------+--------8bits-------+
 |                  type                   |                  len                   |
 +-------8bits--------+-------8bits--------+-------8bits-------+--------8bits-------+
 |                  data...                |
 +-------8bits--------+-------8bits--------+-------8bits-------+--------8bits-------+
*/
struct msg_block {
    struct msg_block *next;
    uint16_t type;
    uint16_t len;
    uint8_t *data;
};

/*
 protocol header schematic:
 32 bits                                   16 bits                                  0 bits
 +---8bits---+--3bits--+--2bits--+--3bits--+-------8bits-------+--------8bits-------+
 | hop_limit | earmark | cfg_hdr |   prio  |                heart_rate              |
 +-------8bits--------+-------8bits--------+-------8bits-------+--------8bits-------+
 |                                      src_id                                      |
 +-------8bits--------+-------8bits--------+-------8bits-------+--------8bits-------+
 |                                      dst_id                                      |
 +-------8bits--------+-------8bits--------+-------8bits-------+--------8bits-------+
 |                   len                   |    check_sum(h)   |    check_sum(l)    |
 +-------8bits--------+-------8bits--------+-------8bits-------+--------8bits-------+
*/
struct msg_header {
    uint8_t hop_limit;
    struct {
        uint8_t earmark: 3;
        uint8_t cfg_hdr: 2;
        uint8_t priority: 3;
    };
    uint16_t heart_rate;
    uint32_t src_id;
    uint32_t dst_id;
    uint16_t len;
    uint16_t check_sum;
};

struct msg_buff {
    struct msg_buff *next;
    struct msg_buff *prev;

    time_t timestamp;
    struct msg_header hdr;
    struct msg_block *blk;
};

int msg_checksum_calc(uint8_t *data);

int msg_buff_dump(void *msgb, uint8_t *data);
int msg_buff_load(void *msgb, uint8_t *data);

void *msg_block_alloc(uint8_t is_micro);
void *msg_header_alloc(uint8_t is_micro);
void *msg_buff_alloc(uint8_t is_micro);

void msg_block_free(void *msg_blk, uint8_t is_micro);
void msg_header_free(void *msg_hdr, uint8_t is_micro);
void msg_buff_free(void *msgb);

int msg_buff_is_micro(uint8_t *data);
int msg_buff_get_hop_limit(uint8_t *data, uint8_t *hop_limit);
int msg_buff_add_hop_limit(uint8_t *data);
int msg_buff_get_prority(uint8_t *data, enum msg_priority *priority);
int msg_buff_set_priority(uint8_t *data, enum msg_priority priority);
int msg_buff_get_heart_rate(uint8_t *data, void *heart_rate);
int msg_buff_set_heart_rate(uint8_t *data, uint16_t heart_rate);
int msg_buff_get_src(uint8_t *data, void *src_id);
int msg_buff_set_src(uint8_t *data, uint32_t src_id);
int msg_buff_get_dst(uint8_t *data, void *dst_id);
int msg_buff_set_dst(uint8_t *data, uint32_t dst_id);
int msg_buff_get_len(uint8_t *data, void *len);
int msg_buff_set_len(uint8_t *data, uint16_t len);
int msg_buff_get_next_block(uint8_t *data, void *next_block);
int msg_buff_set_next_block(uint8_t *data, uint16_t next_block);
int msg_buff_get_checksum(uint8_t *data, uint16_t *checksum);
int msg_buff_set_checksum(uint8_t *data, uint16_t checksum);
int msg_buff_get_payload(uint8_t *data, uint8_t *payload);
int msg_buff_get_block_next(uint8_t *block, void *next_type, uint8_t is_micro);
int msg_buff_get_block_len(uint8_t *block, void *len, uint8_t is_micro);
int msg_buff_get_block_data(uint8_t *block, uint8_t *data, uint8_t is_micro);

#endif /* __PROTO_H__ */
