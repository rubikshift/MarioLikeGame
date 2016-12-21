#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "levelList.h"
#define BUFF_SIZE 20

levelList::levelList()
{
	int counter = 0;
	bool toStop = false;
	const char basicLevelName[] = "level";
	const char prefix[] = ".txt";
	char buff[BUFF_SIZE];


	FILE* fileStream;

	strcpy(buff, basicLevelName);

	do
	{
		strcpy(buff, basicLevelName);
		itoa(counter + 1, &buff[sizeof(basicLevelName) - 1], 10);
		strcat(buff, prefix);

		fileStream = fopen(buff, "r");
		if (fileStream != NULL)
			counter++;
		else
			toStop = true;
	} while (!toStop);

	this->list = new char*[counter];
	for (int i = 0; i < counter; i++)
	{
		this->list[i] = new char[BUFF_SIZE];
		strcpy(buff, basicLevelName);
		itoa(i + 1, &buff[sizeof(basicLevelName) - 1], 10);
		strcat(buff, prefix);
		strcpy(this->list[i], buff);
	}
	this->size = counter;
}

levelList::~levelList()
{
	for (int i = 0; i < this->size; i++)
		delete[] this->list[i];
	delete[] this->list;
}

char* levelList::operator[](int i)
{ return this->list[i]; }