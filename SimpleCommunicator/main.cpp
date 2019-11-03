#include <stdio.h>
#include "pipe.h"
#include <csignal>
#include <pthread.h>
#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <iostream>
 
using namespace std;

int main(int argc, char **argv)
{
	pthread_t threadIdRead;
	pthread_t threadIdWrite;
	typedef void * (*THREADFUNCPTR)(void *);
	Pipe *p = new Pipe();
	if (p->Connect())
	{
		if (p->IsServer())
		{
			printf("Jestem Server\n\n\n\n\n\n\n\n\n\n\n\n\n");
			
			pthread_create(&threadIdRead,NULL,(THREADFUNCPTR)&Pipe::Read,p);
			pthread_create(&threadIdWrite,NULL,(THREADFUNCPTR)&Pipe::Write,p);
			//pthread_join(threadId,NULL);
		}else{
			printf("Jestem Client\n\n\n\n\n\n\n\n\n\n\n\n\n");
			pthread_create(&threadIdRead,NULL,(THREADFUNCPTR)&Pipe::Read,p);
			pthread_create(&threadIdWrite,NULL,(THREADFUNCPTR)&Pipe::Write,p);
			//pthread_join(threadId,NULL);
		}
	
	}
	pthread_join(threadIdRead,NULL);
	printf("Do widzenia\n");
	delete p;
	return 0;
}
