#define _CRT_SECURE_NO_WARNINGS

#include "CPU.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

enum POISON {
	POISON = -666
};

CPU::~CPU() {
	fclose(binFile_);
	binFileSize_ = POISON;
	cmdLenght_ = POISON;
	cmdQt_ = POISON;
	ip_ = POISON;
	for (int i = 0; i < REGISTERS_QUANTITY; i++) {
		registers_[i] = POISON;
	}
	for (int i = 0; i < MAX_ALLOW_FUNC_ARGS; i++) {
		masArgs_[i] = POISON;
	}
	for (int i = 0; i < MAX_ALLOW_FUNC_ARGS; i++) {
		masArgs_[i] = false;
	}
}

#define SCANF(func, arg, code) {													\
	func##(arg, "%[0-9$%] %n", scanString_, &cmdLenght_);							\
	assert(cmdLenght_ <= MAX_ALLOW_SCAN_STRING);									\
	code																			\
}

void CPU::readProgram() {
	getFileSize();
	programString_ = (char *)calloc(binFileSize_, sizeof(char));
	scanString_ = (char *)calloc(MAX_ALLOW_SCAN_STRING, sizeof(char));
	countCmd();
	fread(programString_, sizeof(char), binFileSize_, binFile_);
	for (ip_ = 0; ip_ < cmdQt_; ip_++) {
		SCANF(sscanf, programString_, programString_ += cmdLenght_;)
		CPU_CMDS cmdCode = getCmdNum();
		assert(cmdCode != CPU_DEFAULT);
		int argQt = getCmdArgQt(cmdCode);
		assert(argQt != -1);
		for (int i = 0; i < argQt; i++) {
			analysisArg(i);
		}
		executeCmd(cmdCode);
	}
}

void CPU::getFileSize() {
	fseek(binFile_, 0, SEEK_END);
	binFileSize_ = ftell(binFile_) + 1;
	assert(binFileSize_ > 1);
	rewind(binFile_);
}

void CPU::countCmd() {
	while (!feof(binFile_)) {
		SCANF(fscanf, binFile_, {});
		CPU_CMDS cmdCode = getCmdNum();
		assert(cmdCode != CPU_DEFAULT);
		cmdQt_++;
		int argQt = getCmdArgQt(cmdCode);
		for (int i = 0; i < argQt; i++) {
			SCANF(fscanf, binFile_, {});
		}
	}
	rewind(binFile_);
}

CPU_CMDS CPU::getCmdNum() {
#define CPUCMDS
#define CPUCMD(cmdname, args, code)										\
	if (atof(scanString_) == CPU_##cmdname)								\
		return CPU_##cmdname;
#include "CPU_CMD.txt"
#undef CMUCMD
#undef CMUCMDS
	else return CPU_DEFAULT;
}

int CPU::getCmdArgQt(CPU_CMDS cmdCode) {
#define CPUCMDS
#define CPUCMD(cmdname, args, code)									\
	if (cmdCode == CPU_##cmdname)									\
		return args;
#include "CPU_CMD.txt"
#undef CPUCMD
#undef CPUCMDS
	else return -1;
}

void CPU::analysisArg(int numArg) {
	SCANF(sscanf, programString_, programString_ += cmdLenght_;)
	if (scanString_[0] == '$') {
		masRegOrNum_[numArg] = false;
		deleteSymbol();
	}
	if (scanString_[0] == '%') {
		masRegOrNum_[numArg] = true;
		deleteSymbol();
	}
	else {										//laber
		masRegOrNum_[numArg] = false;
	}
	masArgs_[numArg] = atof(scanString_);
	//printf("%lg\n", masArgs_[numArg]);
}

void CPU::deleteSymbol() {
	int i, j;
	for (i = j = 0; scanString_[i] != '\0'; i++) {
		if (scanString_[i] != '$' && scanString_[i] != '%') {
			scanString_[j++] = scanString_[i];
		}
	}
	scanString_[j] = '\0';
}

void CPU::executeCmd(CPU_CMDS cmdCode) {
	Data_t arg = 0;
	switch (cmdCode)
	{
#define CPUCMDS
#define CPUCMD(cmdname, args, code)						\
	case CPU_##cmdname##:								\
		code											\
		break;											
#include "CPU_CMD.txt"
#undef CPUCMD
#undef CPUCMDS
	default:
		printf("Error\n");
		break;
	}
}

void CPU::cpuDump(FILE* stream) const {
	fprintf(stream, "CPU\n    {\n");
	fprintf(stream, "    cmdQt_ = %i\n", cmdQt_);
	for (int i = 0; i < REGISTERS_QUANTITY; i++) {
		fprintf(stream, "    r[%i] = %lg\n", i, registers_[i]);
	}
	dump(stream, "cpuStack");
	fprintf(stream, "}\n");
}
#undef SCANF