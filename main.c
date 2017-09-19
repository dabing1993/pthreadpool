#include "threadpool.h"
void *process(void *arg)
{
	printf("thread 0x%x working on task %d\n ",(unsigned int)pthread_self(),*(int *)arg);
	sleep(1);
	printf("task %d is end\n",*(int *)arg);

	return NULL;
}

int main(void)
{
	/*threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);*/

	threadpool_t *thp = threadpool_create(3,100,100);/*创建线程池，池里最小3个线程，最大100，队列最大100*/
	printf("pool inited");

	//int *num = (int *)malloc(sizeof(int)*20);
	int num[20], i;
	for (i = 0; i < 20; i++) {
		num[i]=i;
		printf("add task %d\n",i);
		threadpool_add(thp, process, (void*)&num[i]);     /* 向线程池中添加任务 */
	}
	sleep(10);                                          /* 等子线程完成任务 */
	threadpool_destroy(thp);

	return 0;
}
