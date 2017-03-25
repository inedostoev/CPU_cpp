#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h> 

enum ASM_CMDS {
#define ASMCMDS
#define ASMCMD(cmdname, arg)			\
	ASM_##cmdname,									
#include "ASM.txt"
#undef ASMCMD
#undef ASMCMDS
	ASM_DEFAULT
};
enum DTOR_POISON {
	INT_POISON = -666,
};
enum CONFIG {
	MAX_CMD_SIZE	= 10,
	LABELS_QUANTITY = 8
};

class ASM
{
public:
						ASM();
						~ASM();
	void				Compilation();
	FILE				*asmFile_;
	FILE				*binFile_;
private:
	char				*asmProgramString_;
	int					numberLines_;
	char				laberAddress_[LABELS_QUANTITY];

	void				checkFiles();
	ASM_CMDS			getCmdNum();
	void				getNumberLines();
	void				getRegOrNum();
	void				preCompilation();
	int					getCmdArgQt(ASM_CMDS CmdCode);
	void				getArgs(ASM_CMDS CmdCode);
};

ASM::ASM():
	asmFile_			(NULL),
	binFile_			(NULL),
	numberLines_		(0),
	asmProgramString_	((char *)calloc(MAX_CMD_SIZE, sizeof(char)))
{
	for (int i = 0; i < LABELS_QUANTITY; i++) {
		laberAddress_[i] = 0;
	}
}

ASM::~ASM() {
#define FCLOSE(file); {								\
	fclose(file);									\
	file = NULL;    								\
	}
	FCLOSE(asmFile_);
	FCLOSE(binFile_);
#undef FCLOSE
    free(asmProgramString_);
	numberLines_  = INT_POISON;
	for (int i = 0; i < LABELS_QUANTITY; i++) {
		laberAddress_[i] = -666; 
	}
}								

void ASM::Compilation() {
	checkFiles();
	getNumberLines();
	preCompilation();
	for (int i = 0; i < numberLines_; i++) {
		fscanf(asmFile_, "%s", asmProgramString_);
		ASM_CMDS CmdCode = getCmdNum();
		assert(CmdCode != ASM_DEFAULT);
		fprintf(binFile_, "%d ", CmdCode);
		getArgs(CmdCode);
	}
}

void ASM::checkFiles() {
#ifndef NDEBUG
	if (!asmFile_) printf("asmFile no open\n");
	else printf("asmFile open\n");
	if (!binFile_) printf("binFile no open\n");
	else printf("binFile open\n");
#endif
	/*if (!asmFile_) {
		throw( FOPEN_READ_ERROR );
	}
	if (!binFile_) {
	throw( FOPEN_READ_ERROR );
	}*/
}

ASM_CMDS ASM::getCmdNum() {
#define ASMCMDS
#define ASMCMD(cmdname, arg)						\
	if (!strcmp(#cmdname, asmProgramString_))		\
		return ASM_##cmdname;
#include "ASM.txt"
#undef ASMCMD
#undef ASMCMDS
	else return ASM_DEFAULT;
}

void ASM::getNumberLines() {
	while (!feof(asmFile_)) {
		if (fgetc(asmFile_) == '\n') {
			numberLines_++;
		}
	}
	assert(numberLines_ > 0);
	numberLines_++;
	rewind(asmFile_);
}

void ASM::getRegOrNum() {
	char sumbol = fgetc(asmFile_);
	while (sumbol == ' ') {
		sumbol = fgetc(asmFile_);
	}
	if (sumbol == '%' || sumbol == '$') {
		fputc(sumbol, binFile_);
	}
	else {
		fseek(asmFile_, -1, SEEK_CUR);
	}
}

void ASM::preCompilation() {
	// work with laber
}

int ASM::getCmdArgQt(ASM_CMDS CmdCode) {
#define ASMCMDS
#define ASMCMD(cmdname, arg)				\
	if (CmdCode == ASM_##cmdname)			\
		return arg;
#include "ASM.txt"
#undef ASMCMD
#undef ASMCMDS
	else return -1;
}

void ASM::getArgs(ASM_CMDS CmdCode) {
	int argQt = getCmdArgQt(CmdCode);
	assert(argQt != -1);
	for (int i = 0; i < argQt; i++) {
		getRegOrNum();
		int argValue;
		fscanf(asmFile_, "%d", &argValue);
		fprintf(binFile_, "%d ", argValue);
	}
}

int main()
{
	ASM s;
	s.asmFile_ = fopen("./ASM/asmFile.txt", "rb");
	s.binFile_ = fopen("./ASM/binFile.txt", "wb");
	s.Compilation();
	system("PAUSE");
	return 0;
}

/*
void ASM::getFileSize() {
	fseek(asmFile_, 0, SEEK_END);
	fileSize_ = ftell(asmFile_) + 1;
	rewind(asmFile_);
}
*/
