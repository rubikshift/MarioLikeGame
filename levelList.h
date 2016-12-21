#pragma once

class levelList
{
	public:
		char** list;
		int size;

		levelList();
		~levelList();

		char* operator[](int i);
};