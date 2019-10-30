#include "pipe.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h> 
#include <sys/types.h>#
#include <errno.h>


#define FIFO_FILE_PIPE_I "PIPEI"
#define FIFO_FILE_PIPE_II "PIPEII"

Pipe::Pipe(){
	umask(0);
	server=true; 
	openedFiles = false;
};
bool Pipe::Connect(){
	bool ret = CreateFiles();
	
	if (ret)
	{
		openedFiles = true;
		ret = AttachFiles();
	}
	return ret;
};
bool Pipe::AttachFiles(){
	if (server)
	{
		
	}
	else{
	
	}
}
bool Pipe::CreateFiles(){
	bool ret = true;
	int err=0;
	// Create FIFO files, if exist you are client
	err = mkfifo(FIFO_FILE_PIPE_I, 0666);
	if (err == EEXIST)
		server = false; 
	else if(err !=0)
		ret = false; 
	mkfifo(FIFO_FILE_PIPE_II, 0666);
	if (err == EEXIST)
		server = false; 
	else if(err !=0)
		ret = false;
	return ret;
}
Pipe::~Pipe(){
	if (server && openFiles)
	{
		fclose(fpWrite);
		fclose(fpRead);
	}
};