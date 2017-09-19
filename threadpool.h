#ifndef _PTHREADPOOL_H_
#define _PTHREADPOOL_H_

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define DEFAULT_TIME  10
#define MIN_WAIT_TASK_NUM 10
#define DEFAULT_THREAD_VARY 10

#define true 1
#define false 0

typedef struct{
  void *(*function)(void*);
  void *arg;
}threadpool_task_t;

typedef struct threadpool_t
{
  pthread_mutex_t lock;   //lock the struct(self)
  pthread_mutex_t thread_counter;//lock busy thread 
  pthread_cond_t queue_not_full;
  pthread_cond_t queue_not_empty;

  pthread_t *threads;
  pthread_t adjust_tid;
  threadpool_task_t *task_queue;//task queue

  int min_thr_num;//min thread num
  int max_thr_num;//max thread num
  int live_thr_num;//real thread num
  int busy_thr_num;//busy thread num
  int wait_exit_thr_num;//wait thread num

  int queue_front;//front
  int queue_rear;//real
  int queue_size;//real task num
  int queue_max_size;//mx task num

  int shutdown;
}threadpool_t;


/*
*function: threadpool_create
*desc -- create threadpool
*param: min_thr_num --min thread num
*param: max_thr_num --max thread num
*param: queue_max_size --max task queue num
*return: a newly created thread pool or NULL
*/
threadpool_t *threadpool_create(int min_thr_num,int max_thr_num,int queue_max_size);

/*
*function: threadpool_add
*desc -- add a new task in the queue of a thread pool
*param: *pool --thread pool to which add the task
*param: void*(function)(void* arg) --function Pointer to the function that will perform the task
*param: void* arg --argument Argument to be passed to the function
*return 0 if all goes well else -1
*/
int threadpool_add(threadpool_t *pool,void*(*function)(void *arg),void* arg);

/*
*functuion:threadpool_destory
*desc --Stops and destroys a thread pool
*param: pool-- thread pool to destroy
*return 0 if destory success else -1
*/
int threadpool_destroy(threadpool_t *pool);

/*
*function: threadpool_all_threadnum
*desc -- get thread num int the pool
*param: pool -- thread pool to get
*return the num of thread in the thread pool
*/
int threadpool_all_threadnum(threadpool_t *pool);

/*
*function: threadpool_busy_threadnum
*desc -- get thread busy num int the thread pool 
*param: pool -- thread pool to get
*return the num of busy thread in the thread pool
*/
int threadpool_busy_threadnum(threadpool_t *pool);

#endif
