
//demo of P threads

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int g=0;
void *mythfun (void *vargp)
{
	int *myid=(int *)vargp;
	static int s=0;
	int l=0;
	++l;
	++s;
	++g;
	printf("ThreadId =%d, Local =%d, Global=%d and Static =%d\n",*myid,l,g,s);
	pthread_exit(NULL);
}

int main ()
{
	pthread_t tid;
	for(int i =0;i<3;i++)
	{
		pthread_create(&tid,NULL, mythfun, (void *)&tid);
		pthread_join(tid, NULL);
	}
	printf("Main code\n");
	pthread_exit(NULL);
	return 0;
}
