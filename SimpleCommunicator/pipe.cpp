#include "pipe.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h> 
#include <sys/types.h>
#include <errno.h>
#include <string>
#include <iostream>


#define SAVE_CURR_POS "\033[s"
#define RESTORE_CURR_POS "\033[u"
#define MOVE_UP_CURR_POS_10_TIMES "\033[10A"
#define MOVE_UP_CURR_POS_1_TIMES "\033[1A"
#define ERASE_CURR_POS_LINE "\33[2K"
#define WHITE_COLOR "\033[0"
#define RED_COLOR "\033[0;31m"
#define GREAN_COLOR "\033[0;32m"

#define FIFO_FILE_PIPE_I "PIPEI"
#define FIFO_FILE_PIPE_II "PIPEII"
using namespace std;
Pipe::Pipe(){
	umask(0);
	server=false; 
	openedFiles = false;
    sem_init(&mutex, 0, 1); 
	for(int i=0;i<10;i++)
		vOutput.push_back("\n");
};
bool Pipe::Connect(){
	openedFiles = AttachFiles();
	if (!openedFiles)
	{
		server = true;
		CreateFiles();

		openedFiles = AttachFiles();
	}
	return openedFiles;
};
bool Pipe::AttachFiles(){
	if (server)
	{
		fpWrite = fopen(FIFO_FILE_PIPE_I,"w");
		fpRead = fopen(FIFO_FILE_PIPE_II,"r");
	}
	else{
		fpRead = fopen(FIFO_FILE_PIPE_I,"r");
		if (fpRead != NULL)
			fpWrite = fopen(FIFO_FILE_PIPE_II,"w");
	}
	if (fpRead == NULL)
		return false;
	return true;
}
bool Pipe::CreateFiles(){
	bool ret = true;
	int err=0;
	// Create FIFO files, if exist you are client
	err = mkfifo(FIFO_FILE_PIPE_I, 0666);
	if(err !=0)
		ret = false; 
	err = mkfifo(FIFO_FILE_PIPE_II, 0666);
	if(err !=0)
		ret = false;
	return ret;
}
void Pipe::Read(){
	Reprint(true);
	fflush(stdout);
	char readbuf[80];
	bool fileOpen=true;
	string buf;
	while(fileOpen)
	{
		buf = "";
		if (fgets(readbuf, 80, fpRead) == NULL)
		{
			fileOpen = false;
			continue;
		}
		buf += readbuf;
		if (buf != "")
		{
			std::size_t found = buf.find('\n');
			if(found == string::npos)
				buf +="\n";
			buf = "<<" + buf; 
			Print(buf);
		}
	}
	printf("Koniec polaczenia\n");
}
void Pipe::Write(){
	int retval=0;
	string buf ="";
	bool fileOpen=true;
	while(fileOpen)
	{
		fd_set rfds;
		struct timeval tv;
		buf ="";
	/* Watch stdin (fd 0) to see when it has input. */
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
	/* Wait up to five seconds. */
		tv.tv_sec = 5;
		tv.tv_usec = 0;

		retval = select(1, &rfds, NULL, NULL, &tv);
		if(retval == 0);
		else if(retval < 0)
			buf = "Koniec polaczenia\n";
		else
			{
			getline(cin,buf);
			buf+="\n";
		}
		if (buf!="")
		{
			fputs(buf.c_str(),fpWrite);
			fflush(fpWrite);
			buf = ">>" + buf; 
			Print(buf,true);
		}
	}
}
bool Pipe::IsServer(){
	return server;
}

void Pipe::Print(string line,bool eraseInput){
	
	vector<string> vTempOutput;
    sem_wait(&mutex); 
	vTempOutput.push_back(line);
	for(int i=0; i<vOutput.size();++i)
	{
		vTempOutput.push_back(vOutput[i]);
	}
	vOutput.clear();
	for(int i =0; i<=10; i++)
	{
		vOutput.push_back(vTempOutput[i]);
	}
	Reprint(eraseInput);
    sem_post(&mutex); 
}
void Pipe::Reprint(bool eraseInput){
	string output; 
	printf(SAVE_CURR_POS);
	
	output += MOVE_UP_CURR_POS_10_TIMES;
	if(eraseInput)
		printf(MOVE_UP_CURR_POS_1_TIMES);
	output += ERASE_CURR_POS_LINE;
	output += "\r";
	
	for(int i = 9; i>=0; --i)
	{
		output +=ERASE_CURR_POS_LINE;
		output +=vOutput[i];
	}
	printf(output.c_str());
	printf(RESTORE_CURR_POS);
	if(eraseInput)
	{
		printf(MOVE_UP_CURR_POS_1_TIMES);
		printf(ERASE_CURR_POS_LINE);
	}
	fflush(stdout);
}
Pipe::~Pipe(){
	if (openedFiles)
	{
		fclose(fpWrite);
		fclose(fpRead);
		string pathName = "./";
		pathName += FIFO_FILE_PIPE_I;
		unlink(pathName.c_str());
		pathName = "./";
		pathName += FIFO_FILE_PIPE_II;
		unlink(pathName.c_str());
	}
    sem_destroy(&mutex); 
};