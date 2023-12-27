#ifndef SHM_QUEUE_H_
#define SHM_QUEUE_H_

#include <stdint.h>
#include <stddef.h>

typedef struct queue_s{
    byte   *memory;
    byte   *read_buf;
    size_t read_buf_size;
} queue_t;

int queue_init(queue_t *queue, key_t shm_key, uint mem_size, char *file, size_t file_max_size);

int queue_push(queue_t *queue, void *data, uint size);
int queue_pop(queue_t *queue, void **data, uint *size);

void queue_fini(queue_t *queue);

typedef struct queue_info_t {
    uint mem_num;
    uint mem_size;
    size_t file_num;
    size_t file_size;
} queue_info;

void queue_state(queue_t *queue, queue_info *info);

#endif

