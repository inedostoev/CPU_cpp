#define _CRT_SECURE_NO_WARNINGS
#define NDEBUG

#include "CPU.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MYASSERT(condition, error)      \
    if(!(condition)) {                  \
        cpuDumpErr(stdout, error);      \
        assert(condition);              \
    }

enum POISON {
	POISON = -666
};

CPU::CPU() :
    binFile_                (NULL),
    slider_                 (NULL),
    firstProgramStrPointer_ (NULL),
    scannedString_          ((char*)calloc(MAX_ALLOW_SCAN_STRING, sizeof(char))),
    binFileSize_            (0),
    cmdLength_              (0),
    cmdQt_                  (0),
    ip_                     (0),
    stack_                  ({})
{
    for (int i = 0; i < REGISTERS_QUANTITY; i++) {
        registers_[i] = 0;        
    }
    for (int i = 0; i < MAX_ALLOW_FUNC_ARGS; i++) {
      masArgs_[i] = 0;
      masRegOrNum_[i] = false;
    }
}

CPU::~CPU() {
	fclose(binFile_);
	free(scannedString_);
    binFile_ = NULL;
	binFileSize_ = POISON;
	cmdLength_ = POISON;
	cmdQt_ = POISON;
	ip_ = POISON;	
	for (int i = 0; i < REGISTERS_QUANTITY; i++) {
		registers_[i] = POISON;
	}
	for (int i = 0; i < MAX_ALLOW_FUNC_ARGS; i++) {
		masArgs_[i] = POISON;
        masRegOrNum_[i] = false;
	}
}
  
#define SCANF(func, arg, code) {                            				\
	assert(MAX_ALLOW_SCAN_STRING == 11);                                    \
    int count = func(arg, "%10[0-9$%.-] %n", scannedString_, &cmdLength_);  \
    assert(count < 11);                                                     \
	code						                                            \
}

void CPU::readProgram() {
	MYASSERT(binFile_, ERROR_FOPEN_FILE);
    getFileSize();
	char *programString = (char *)calloc(binFileSize_, sizeof(char));
    assert(programString != NULL);
    slider_ = programString;
    firstProgramStrPointer_ = programString;
	countCmd();
    size_t size = fread(slider_, sizeof(char), binFileSize_, binFile_);
    assert(size <= binFileSize_);
    for (ip_ = 0; ip_ < cmdQt_; ip_++) {				
		SCANF(sscanf, slider_, slider_ += cmdLength_;)
		CPU_CMDS cmdCode = getCmdNum();
        assert(cmdCode != CPU_DEFAULT);
        int argQt = getCmdArgQt(cmdCode);
		assert(argQt != -1 && argQt <= MAX_ALLOW_FUNC_ARGS);
		for (int i = 0; i < argQt; i++) {
			analysisArg(i);
		}
		executeCmd(cmdCode);
	}
    free(programString);
    slider_ = NULL;
    firstProgramStrPointer_ = NULL;
}

void CPU::getFileSize() {
	fseek(binFile_, 0, SEEK_END);
	binFileSize_ = ftell(binFile_) + 1;
    MYASSERT(binFileSize_ > 1, ERROR_EMPTY_FILE);
	rewind(binFile_);
}

void CPU::countCmd() {
	while (!feof(binFile_)) {
		SCANF(fscanf, binFile_, {});
		CPU_CMDS cmdCode = getCmdNum();
        assert(cmdCode != CPU_DEFAULT);
		cmdQt_++;
		int argQt = getCmdArgQt(cmdCode);
        assert(argQt != -1 && argQt <= MAX_ALLOW_FUNC_ARGS);
		for (int i = 0; i < argQt; i++) {
			SCANF(fscanf, binFile_, {});
		}
	}
	rewind(binFile_);
}

CPU_CMDS CPU::getCmdNum() {
double tmp = atof(scannedString_);
#define CPUCMDS
#define CPUCMD(cmdname, args, code)									            \
    if ((CPU_##cmdname - tmp) < ACCURACY && (tmp - CPU_##cmdname) < ACCURACY)   \
		return CPU_##cmdname;
#include "./ASM/CPU_CMD.txt"
#undef CPUCMD
#undef CPUCMDS
	else return CPU_DEFAULT;
}

int CPU::getCmdArgQt(CPU_CMDS cmdCode) {
#define CPUCMDS
#define CPUCMD(cmdname, args, code)									            \
    if (cmdCode == CPU_##cmdname)									            \
        return args;
#include "./ASM/CPU_CMD.txt"
#undef CPUCMD
#undef CPUCMDS
	else return -1;
}

void CPU::analysisArg(int numArg) {
	SCANF(sscanf, slider_, slider_ += cmdLength_;)
	if (scannedString_[0] == '$') {
	    masRegOrNum_[numArg] = false;
		deleteSymbol();
	}
	if (scannedString_[0] == '%') {
		masRegOrNum_[numArg] = true;
		deleteSymbol();
	}
	else {										
		masRegOrNum_[numArg] = false;
	}
	masArgs_[numArg] = atof(scannedString_);
}

void CPU::deleteSymbol() {
	int i, j;
	for (i = j = 0; scannedString_[i] != '\0'; i++) {
		if (scannedString_[i] != '$' && scannedString_[i] != '%') {
			scannedString_[j++] = scannedString_[i];
		}
	}
	scannedString_[j] = '\0';
}

void CPU::executeCmd(CPU_CMDS cmdCode) {
	Data_t arg = 0;
    Data_t secondArg = 0;
	switch (cmdCode)
	{
#define CPUCMDS
#define CPUCMD(cmdname, args, code)						\
	case CPU_##cmdname:									\
		code											\
		break;											
#include "./ASM/CPU_CMD.txt"
#undef CPUCMD
#undef CPUCMDS
	default:
		printf("Error, unknown cmd\n");
		break;
	}
}

void CPU::moveIp(Data_t newAddress) {
    slider_ = firstProgramStrPointer_;
    for(ip_ = 0; ip_ < newAddress; ip_++) {
        SCANF(sscanf, slider_, slider_ += cmdLength_;)
		CPU_CMDS cmdCode = getCmdNum();
        assert(cmdCode != CPU_DEFAULT);
		int argQt = getCmdArgQt(cmdCode);
        assert(argQt != -1 && argQt <= MAX_ALLOW_FUNC_ARGS);
		for (int i = 0; i < argQt; i++) {
			SCANF(sscanf, slider_, slider_ += cmdLength_;)
		}
    }
}

void CPU::cpuDump(FILE* stream) const {
	fprintf(stream, "CPU\n    {\n");
	fprintf(stream, "    cmdQt_ = %i\n", cmdQt_);
	for (int i = 0; i < REGISTERS_QUANTITY; i++) {
		fprintf(stream, "    r[%i] = %lg\n", i, registers_[i]);
	}
    char nameStack[9] = "cpuStack";
    char *pointerNameStack = nameStack;
    assert(pointerNameStack != NULL);
	stack_.dump(stream, nameStack);
	fprintf(stream, "}\n");
}

void CPU::cpuDumpErr(FILE *stream, CPU_ERRORS error) {
#define CPUERRORS
#define CPUERROR(nameError, message)                                \
    if(nameError == error)                                          \
        fprintf(stream, message);
#include "CPU_ERROR.txt"
#undef CPUERROR
#undef CPUERRORS    
}
#undef SCANF
#undef MYASSERT
