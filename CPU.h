#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED
#include "./Stack/Stack.h"

typedef double Data_t;

enum PROCESSOR_PROPERIES
{
	REGISTERS_QUANTITY = 4,
	MAX_ALLOW_SCAN_STRING = 11,
	MAX_ALLOW_FUNC_ARGS = 3,
};

enum CPU_CMDS{
#define CPUCMDS
#define CPUCMD(cmdname, args, code)									\
	CPU_##cmdname,
#include "./ASM/CPU_CMD.txt"
#undef CPUCMD
#undef CPUCMDS
	CPU_DEFAULT
};

class CPU
{
public:
				CPU();
				~CPU();
	FILE		*binFile_;
	void		readProgram();
	void		cpuDump(FILE* stream) const;
private:
    char        *slider_;
    char        *firstProgramStrPointer_;
	char        *scannedString_;
    size_t  	binFileSize_;
	int			cmdLength_;
	int			cmdQt_;
	int			ip_;
	Stack       stack_;  

	Data_t		registers_[REGISTERS_QUANTITY];
	Data_t		masArgs_[MAX_ALLOW_FUNC_ARGS];	
    bool        masRegOrNum_[MAX_ALLOW_FUNC_ARGS];

	void		getFileSize();
	void		countCmd();
	CPU_CMDS	getCmdNum();
	int			getCmdArgQt(CPU_CMDS cmdCode);
	void		analysisArg(int numArg);
	void		deleteSymbol();
	void		executeCmd(CPU_CMDS cmdCode);
    void        moveIp(double newAddress);
};
#endif
