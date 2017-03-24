#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "CPU.h"

int main()
{
	CPU cpu(6);
	cpu.binFile_ = fopen("binFile.txt", "rb");
	cpu.readProgram();

	cpu.cpuDump(stdout);
	FILE*  stream = fopen("dump.txt", "a");
	cpu.cpuDump(stream);
	fclose(stream);
	
	cpu.~CPU();
	system("PAUSE");
	return 0;
}