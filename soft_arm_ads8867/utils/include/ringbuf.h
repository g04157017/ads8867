#ifndef JSCB_RINGBUF_H
#define JSCB_RINGBUF_H

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#define RINGBUF_MAX_NUM  (60*1024)
#define RINGBUF_MIN_NUM  (30*1024)

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t  putcond;
    pthread_cond_t  getcond;
    ulong* putptr;
    ulong* getptr;
    ulong* buff;
    ulong  size;
} RBUFF_T;

#define RBUF_INIT(rt, rb, rs)                                                  \
    do {                                                                       \
        (rb) = (rt*)malloc(sizeof(rt));                                        \
        if (rb) {                                                              \
            (rb)->buff = (ulong*)malloc(rs*sizeof(ulong));                     \
            if ((rb)->buff) {                                                  \
                pthread_mutex_init(&(rb)->mutex,  NULL);                       \
                pthread_cond_init(&(rb)->putcond, NULL);                       \
                pthread_cond_init(&(rb)->getcond, NULL);                       \
                (rb)->putptr = (rb)->buff;                                     \
                (rb)->getptr = (rb)->buff;                                     \
                (rb)->size   = rs;                                             \
            } else {                                                           \
                free(rb);                                                      \
                (rb) = NULL;                                                   \
            }                                                                  \
        }                                                                      \
    } while (false)

#define RBUF_PUSH(rb, e)                                                       \
    do {                                                                       \
        pthread_mutex_lock(&(rb)->mutex);                                      \
        while (((rb)->putptr + 1) == (rb)->getptr) {                           \
            pthread_cond_wait(&(rb)->putcond, &(rb)->mutex);                   \
        }                                                                      \
        if (((rb)->putptr + 1) > ((rb)->buff + (rb)->size)) {                  \
            (rb)->putptr = (rb)->buff;                                         \
        }                                                                      \
        *(rb)->putptr = (ulong)e;                                              \
        (rb)->putptr++;                                                        \
        pthread_mutex_unlock(&(rb)->mutex);                                    \
        pthread_cond_signal(&(rb)->getcond);                                   \
    } while (false)

#define RBUF_POP(rb, e, rt)                                                    \
    do {                                                                       \
        pthread_mutex_lock(&(rb)->mutex);                                      \
        while (((rb)->getptr) == (rb)->putptr) {                               \
            pthread_cond_wait(&(rb)->getcond, &(rb)->mutex);                   \
        }                                                                      \
        if (((rb)->getptr + 1) > ((rb)->buff + (rb)->size)) {                  \
            (rb)->getptr = (rb)->buff;                                         \
        }                                                                      \
        e = (rt*)*((rb)->getptr);                                              \
        (rb)->getptr++;                                                        \
        pthread_mutex_unlock(&(rb)->mutex);                                    \
        pthread_cond_signal(&(rb)->putcond);                                   \
    } while (false)

#define RBUF_UNLOCK(rb)                                                        \
    pthread_mutex_unlock((rb)->mutex);

#define RBUF_FREE(rb)                                                          \
    do {                                                                       \
        if (rb) {                                                              \
            pthread_mutex_destroy(&(rb)->mutex);                               \
            pthread_cond_destroy(&(rb)->putcond);                              \
            pthread_cond_destroy(&(rb)->getcond);                              \
            free(rb->buff);                                                    \
            free(rb);                                                          \
            (rb) = NULL;                                                       \
        }                                                                      \
    } while (false)

#endif /* RING_BUF_H */
