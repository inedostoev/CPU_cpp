#define _CRT_SECURE_NO_WARNINGS
#define NDEBUG

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h> 

enum ASM_CMDS {
#define CPUCMDS
#define CPUCMD(cmdname, arg, code)			\
	ASM_##cmdname,									
#include "CPU_CMD.txt"
#undef CPUCMD
#undef CPUCMDS
	ASM_DEFAULT
};
enum DTOR_POISON {
	INT_POISON = -666,
};
enum CONFIG {
	MAX_CMD_SIZE	= 11,
	MAX_ARG_SIZE    = 11,
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
	char				*slider_;
    size_t              fileSize_;
	int					numberLines_;
	int 				labelAddress_[LABELS_QUANTITY];
	char				*scannedString_;
	int					cmdLength_;
    int                 cmdQt_;

	void				checkFiles();
    void                getFileSize(); 
	void				gerCmdQt();
	ASM_CMDS			getCmdNum();
	void				preCompilation();
	int					getCmdArgQt(ASM_CMDS CmdCode);
	void				getArgs(ASM_CMDS CmdCode);
};

ASM::ASM():
	asmFile_			(NULL),
	binFile_			(NULL),
    slider_             (NULL),
    fileSize_           (0),
    numberLines_        (0),
	scannedString_		((char*)calloc(MAX_CMD_SIZE, sizeof(char))),
	cmdLength_			(0),
    cmdQt_              (0)
{
    for (int i = 0; i < LABELS_QUANTITY; i++) {
		labelAddress_[i] = -1;
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
    free(scannedString_);
    numberLines_  = INT_POISON;
    fileSize_ = INT_POISON;
    cmdLength_ = INT_POISON;
    cmdQt_ = INT_POISON;
	for (int i = 0; i < LABELS_QUANTITY; i++) {
		labelAddress_[i] = -666; 
	}
}								

void ASM::Compilation() {
	checkFiles();
	getFileSize();
    char *asmProgramString = (char*)calloc(fileSize_, sizeof(char));
    slider_ = asmProgramString;
    size_t count = fread(slider_, sizeof(char), fileSize_, asmFile_);
    assert(count = fileSize_);
    preCompilation();   
    for (int i = 0; i < cmdQt_; i++) {
		assert(MAX_CMD_SIZE == 11);
		sscanf(slider_, "%10[A-Z0-9] %n", scannedString_, &cmdLength_);
		slider_ += cmdLength_;
		ASM_CMDS CmdCode = getCmdNum();
		assert(CmdCode != ASM_DEFAULT);
		fprintf(binFile_, "%d ", CmdCode);
		getArgs(CmdCode);
	}
    free(asmProgramString);
    slider_ = NULL;
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

void ASM::getFileSize() {
    fseek(asmFile_, 0, SEEK_END);
    fileSize_ = (size_t)ftell(asmFile_) + 1;
    assert(fileSize_ > 1);
    rewind(asmFile_);
}

void ASM::gerCmdQt() {
	char *strPointer = slider_;
	while (strPointer != slider_ + fileSize_ - 1) {
		sscanf(strPointer, "%10[A-Z0-9$%.-] %n", scannedString_, &cmdLength_);
		strPointer += cmdLength_;
		ASM_CMDS cmdCode = getCmdNum();
		assert(cmdCode != ASM_DEFAULT);
		cmdQt_++;
		int argQt = getCmdArgQt(cmdCode);
		for (int i = 0; i < argQt; i++) {
			sscanf(strPointer, "%10[0-9%$.-] %n", scannedString_, &cmdLength_);
			strPointer += cmdLength_;
		}
	}
	strPointer = NULL;
}

ASM_CMDS ASM::getCmdNum() {
#define CPUCMDS
#define CPUCMD(cmdname, arg, code)						\
	if (!strcasecmp(#cmdname, scannedString_))  	    \
		return ASM_##cmdname;
#include "CPU_CMD.txt"
#undef CPUCMD
#undef CPUCMDS
	else return ASM_DEFAULT;
}

void ASM::preCompilation() {
	int label;
    char *strPointer;
    for (unsigned int i = 0; i < fileSize_; i++) {
        if(slider_[i] == '\n') {
            numberLines_++;    
            }
        if(slider_[i] == ':') {
            label = slider_[i + 1] - '0';
			assert(LABELS_QUANTITY <= 9 && label <LABELS_QUANTITY && labelAddress_[label] == -1);
            labelAddress_[label] = numberLines_ ;

			for (strPointer = slider_ + i; strPointer < strchr(strPointer, '\n'); strPointer++) {
				*strPointer = ' ';
			}
			*strPointer = ' ';
		}

        if(slider_[i] == ';') {
			for (strPointer = slider_ + i; strPointer < strchr(strPointer, '\n'); strPointer++) {
				*strPointer = ' ';
			}
        }
    }
	gerCmdQt();
	strPointer = NULL;
}

int ASM::getCmdArgQt(ASM_CMDS CmdCode) {
#define CPUCMDS
#define CPUCMD(cmdname, arg, code)				        \
    if (CmdCode == ASM_##cmdname)   			        \
        return arg;
#include "CPU_CMD.txt"
#undef CPUCMD
#undef CPUCMDS
	else return -1;
}

void ASM::getArgs(ASM_CMDS CmdCode) {
	int argQt = getCmdArgQt(CmdCode);
	assert(argQt != -1);
	char *argValue = (char*)calloc(MAX_ARG_SIZE, sizeof(char));
	for (int i = 0; i < argQt; i++) {
		assert(MAX_ARG_SIZE == 11);
		sscanf(slider_, "%10[0-9%$.-] %n", argValue, &cmdLength_);
		slider_ += cmdLength_;
        if (argValue[0] == '$' || argValue[0] == '%') {
		fprintf(binFile_, "%s ", argValue);
        }
        else {
            assert(labelAddress_[argValue[0] - '0'] != -1);
            fprintf(binFile_, "%d ", labelAddress_[argValue[0] - '0']);
        }
    }
    free(argValue);
}

int main() {
	ASM s;
	s.asmFile_ = fopen("asmFile.txt", "rb");
	s.binFile_ = fopen("binFile.txt", "wb");
	s.Compilation();
	return 0;
}
