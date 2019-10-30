#include <stdio.h>
#include "pipe.h"

int main(int argc, char **argv)
{
	Pipe *p = new Pipe();
	printf("hello world\n");
	delete p;
	return 0;
}
