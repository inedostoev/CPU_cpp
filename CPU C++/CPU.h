#pragma once
#include "Stack.h"

typedef double Data_t;

enum REGISTERS {
	REGISTERS_QUANTITY = 4
};

enum {
	MAX_ALLOW_SCAN_STRING = 10,
	MAX_ALLOW_FUNC_ARGS = 3,
};

enum CPU_CMDS{
#define CPUCMDS
#define CPUCMD(cmdname, args, code)									\
	CPU_##cmdname,
#include "CPU_CMD.txt"
#undef CMUCMD
#undef CMUCMDS
	CPU_DEFAULT
};

class CPU : private Stack
{
public:
				CPU::CPU(size_t cap) : Stack(cap) {
					binFile_	 =		NULL;
					binFileSize_ =		0;
					cmdLenght_	 =		0;
					cmdQt_       =		0;
					ip_          =		0;
					for (int i = 0; i < REGISTERS_QUANTITY; i++) {
						registers_[i] = 0;
					}
					for (int i = 0; i < MAX_ALLOW_FUNC_ARGS; i++) {
						masArgs_[i] = 0;
					}
					for (int i = 0; i < MAX_ALLOW_FUNC_ARGS; i++) {
						masArgs_[i] = false;
					}
				}
				~CPU();
	FILE		*binFile_;
	void		readProgram();
	void		cpuDump(FILE* stream) const;
private:
	char		*programString_;
	int			binFileSize_;
	char		*scanString_;
	int			cmdLenght_;
	int			cmdQt_;
	int			ip_;
	
	Data_t		registers_[REGISTERS_QUANTITY];
	Data_t		masArgs_[MAX_ALLOW_FUNC_ARGS];
	bool		masRegOrNum_[MAX_ALLOW_FUNC_ARGS];

	void		getFileSize();
	void		countCmd();
	CPU_CMDS	getCmdNum();
	int			getCmdArgQt(CPU_CMDS cmdCode);
	void		analysisArg(int numArg);
	void		deleteSymbol();
	void		executeCmd(CPU_CMDS cmdCode);
};

