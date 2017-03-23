#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "CPU.h"
#include "Stack.h"	
#include "CPU.cpp"
#include "Stack.cpp"

using namespace std;

int main()
{
	CPU s(5);
	FILE*  str;
	str = fopen("commands.txt", "r");
	s.convector(str);
		
	fclose(str);
	system("PAUSE");
	return 0;
}