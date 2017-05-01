#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "common.h"

int main()
{
	int fd = shm_open("shm1", O_CREAT | O_RDWR, 0644);
	if (fd < 0)
	{
		printf("shm_open error\n");
		exit(-1);
	}
		
	int len = sizeof(struct Test);
	ftruncate(fd, len);
	struct Test* p = (struct Test*)mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	pthread_mutexattr_t mattr;
	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&p->m1, &mattr);
	pthread_mutex_init(&p->m2, &mattr);
	
	while(1)
	{
		pthread_mutex_lock(&p->m2);
		pthread_mutex_lock(&p->m1);
		//sleep(1);
		printf("progress2\n");
		pthread_mutex_unlock(&p->m1);
		pthread_mutex_unlock(&p->m2);
	}
	
	pthread_mutex_destroy(&p->m1);
	pthread_mutex_destroy(&p->m2);
	return 0;
}
