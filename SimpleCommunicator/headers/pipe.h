#pragma once
#include <stdio.h>


class Pipe{
public:
	Pipe();
	bool Connect();
	~Pipe();
private:
	bool CreateFiles();
	bool AttachFiles();
	FILE *fpWrite; 
	FILE *fpRead; 
	char readBuf[128];
	bool server; 
	bool openedFiles;
};