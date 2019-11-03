#pragma once
#include <stdio.h>
#include <string>
#include <semaphore.h> 
#include <vector>


class Pipe{
public:
	Pipe();
	bool Connect();
	void Read();
	void Write();
	bool IsServer();
	void Print(std::string line,bool eraseInput = false);
	~Pipe();
private:
	void Reprint(bool eraseInput = false);
	sem_t mutex; 
	bool CreateFiles();
	bool AttachFiles();
	FILE *fpWrite; 
	FILE *fpRead; 
	char readBuf[128];
	bool server; 
	bool openedFiles;
	std::vector<std::string> vOutput;
};