#ifndef JSCB_QUEUE_H
#define JSCB_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#define QUEUE_MAX_NUM  32*1024
#define QUEUE_MIN_NUM  10*1024

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_cond_t recvcond;
    void* front;
    void* back;
    void* backqh;
    unsigned int size;
} queue_core;

typedef struct queue_handle {
    queue_core* qc;
    void* next;
} queue_handle_t;

#define QUEUE_INIT(qt, q)                                                      \
    do {                                                                       \
        queue_core* qc;                                                        \
        (q) = (qt*)malloc(sizeof(qt));                                         \
        if (q) {                                                               \
            (q)->qh.qc = (queue_core*)malloc(sizeof(queue_core));              \
            if ((q)->qh.qc) {                                                  \
                qc = (q)->qh.qc;                                               \
                pthread_mutex_init(&qc->mutex, NULL);                          \
                pthread_cond_init(&qc->cond, NULL);                            \
                pthread_cond_init(&qc->recvcond, NULL);                        \
                qc->front = qc->back = NULL;                                   \
                qc->backqh = NULL;                                             \
                qc->size = 0;                                                  \
                (q)->qh.next = NULL;                                           \
            } else {                                                           \
                free(q);                                                       \
                (q) = NULL;                                                    \
            }                                                                  \
        }                                                                      \
    } while (false)

#define QUEUE_PUSH(q, e)                                                       \
    do {                                                                       \
        queue_core* qc;                                                        \
        queue_handle* backqh;                                                  \
        qc = (q)->qh.qc;                                                       \
        pthread_mutex_lock(&qc->mutex);                                        \
        while (QUEUE_SIZE(q) > QUEUE_MIN_NUM) {                                \
            pthread_cond_wait(&qc->recvcond, &qc->mutex);                      \
        }                                                                      \
        (e)->qh.qc = qc;                                                       \
        (e)->qh.next = NULL;                                                   \
        backqh = (queue_handle*)qc->backqh;                                    \
        if (!qc->front) {                                                      \
            qc->front = qc->back = (e);                                        \
        } else {                                                               \
            backqh->next = (e);                                                \
            qc->back = (e);                                                    \
        }                                                                      \
        backqh = &((e)->qh);                                                   \
        qc->backqh = backqh;                                                   \
        qc->size++;                                                            \
        pthread_mutex_unlock(&qc->mutex);                                      \
        pthread_cond_signal(&qc->cond);                                        \
    } while (false)

#define MAX_QUEUE_PUSH(q, e)                                                   \
    do {                                                                       \
        queue_core* qc;                                                        \
        queue_handle* backqh;                                                  \
        qc = (q)->qh.qc;                                                       \
        pthread_mutex_lock(&qc->mutex);                                        \
        while (QUEUE_SIZE(q) > QUEUE_MAX_NUM) {                                \
            pthread_cond_wait(&qc->recvcond, &qc->mutex);                      \
        }                                                                      \
        (e)->qh.qc = qc;                                                       \
        (e)->qh.next = NULL;                                                   \
        backqh = (queue_handle*)qc->backqh;                                    \
        if (!qc->front) {                                                      \
            qc->front = qc->back = (e);                                        \
        } else {                                                               \
            backqh->next = (e);                                                \
            qc->back = (e);                                                    \
        }                                                                      \
        backqh = &((e)->qh);                                                   \
        qc->backqh = backqh;                                                   \
        qc->size++;                                                            \
        pthread_mutex_unlock(&qc->mutex);                                      \
        pthread_cond_signal(&qc->cond);                                        \
    } while (false)

#define QUEUE_POP(q, e, qt)                                                    \
    do {                                                                       \
        queue_core* qc;                                                        \
        (e) = NULL;                                                            \
        qc = (q)->qh.qc;                                                       \
        pthread_mutex_lock(&qc->mutex);                                        \
        while (QUEUE_SIZE(q) == 0) {                                           \
            pthread_cond_wait(&qc->cond, &qc->mutex);                          \
        }                                                                      \
        if ((q)->qh.qc->front != NULL) {                                       \
            (e) = (qt*)(q)->qh.qc->front;                                      \
            (q)->qh.qc->front = (e)->qh.next;                                  \
            (q)->qh.qc->size--;                                                \
        }                                                                      \
        pthread_mutex_unlock(&qc->mutex);                                      \
        pthread_cond_signal(&qc->recvcond);                                    \
    } while (false)

#define QUEUE_LOCK(q)                                                          \
    pthread_mutex_lock(&q->qh.qc->mutex);

#define QUEUE_SIZE(q)                                                          \
    ((q)->qh.qc->size)

#define QUEUE_UNLOCK(q)                                                        \
    pthread_mutex_unlock(&q->qh.qc->mutex);

#define QUEUE_FREE(q)                                                          \
    do {                                                                       \
        queue_core* qc;                                                        \
        if ((q) && (q)->qh.qc) {                                               \
            qc = (q)->qh.qc;                                                   \
            pthread_cond_destroy(&qc->cond);                                   \
            pthread_cond_destroy(&qc->recvcond);                               \
            pthread_mutex_destroy(&qc->mutex);                                 \
            free(qc);                                                          \
            free(q);                                                           \
        }                                                                      \
    } while (false)

#endif /* QUEUE_H */
