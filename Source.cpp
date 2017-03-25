#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "CPU.h"
#include "Stack.h"

int main()
{
	CPU cpu;
	cpu.binFile_ = fopen("./ASM/binFile.txt", "rb");
    assert(cpu.binFile_);
	cpu.readProgram();

	cpu.cpuDump(stdout);
	FILE*  stream = fopen("dump.txt", "a");
	cpu.cpuDump(stream);
	fclose(stream);

	return 0;
}
