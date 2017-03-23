#include "CPU.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


enum {
	add_� = 1000,
	sub_� = 1001,
	mal_� = 1010,
	div_� = 1011,
	hlt_� = -1,
	out_� = 2001,
	in_� = 2010,
	push_� = 3001,
	push_x = 3010,
	pop_� = 4001,
	pop_x = 4010,
	ax = 1,
	bx = 2,
	cx = 3,
	dx = 4
};

CPU::~CPU() {

}

#define CPU_command(func, com);									\
		case com:												\
		func();													\
		break;										

void CPU::convector(FILE *stream) {
	while (!feof(stream)) {
		if (fgetc(stream) == ' ') {
			max_ip++;
		}
	}
	rewind(stream);
	if (max_ip < 10) {
		resize_mas(max_ip);
	}
	for (int i = 0; i < max_ip; i++) {
		fscanf_s(stream, "%d", &commands_[i]);
	}
	for (int i = 0; i < max_ip; i++) {
		printf("%d\n", commands_[i]);
	}
	while (ip_ < max_ip) {
		switch (commands_[ip_])
		{
			CPU_command(add, add_�);
			CPU_command(sub, sub_�);
			CPU_command(mal, mal_�);
			CPU_command(div, div_�);
		case hlt_�:
			hlt(max_ip);
			break;
			CPU_command(out, out_�);
			CPU_command(in, in_�);
		case push_�:
			Stack::push(commands_[++ip_]);
			break;
		case push_x:
			switch (commands_[++ip_]) {
			case ax:
				push(&ax_);
				break;
			case bx:
				push(&bx_);
				break;
			case cx:
				push(&cx_);
				break;
			case dx:
				push(&dx_);
				break;
			}
		case pop_�:
			Stack::pop();
			break;
		case pop_x:
			switch (commands_[++ip_]) {
			case ax:
				pop(&ax_);
				break;
			case bx:
				pop(&bx_);
				break;
			case cx:
				pop(&cx_);
				break;
			case dx:
				pop(&dx_);
				break;
			}
			break;
		}
		ip_++;
	}
	printf("%lg %lg %lg %lg\n", ax_, bx_, cx_, dx_);
#define DUMP(file,Stack_name);							\
{														\
	Stack::dump(file,#Stack_name);					    \
}

	DUMP(stdout, stack);
}
#undef CPU_command(func, com);

void CPU::resize_mas(int max_ip) {
	assert(commands_ != NULL);
	commands_ = (int *)realloc(commands_, max_ip * sizeof(int));
}

void CPU::add() {
	Stack::push(Stack::pop() + Stack::pop());
}

void CPU::sub() {
	Data_t i = Stack::pop();
	Stack::push(i - Stack::pop());
}

void CPU::mal() {
	Stack::push(Stack::pop() * Stack::pop());
}

void CPU::div() {
	Data_t i = Stack::pop();
	Stack::push(i / Stack::pop());
}

void CPU::hlt(int max) {
	ip_ = max - 1;
}

void CPU::out() {
	printf("%lg\n", Stack::pop());
}

void CPU::in() {
	Data_t i = 0;
	printf("#Print number");
	scanf_s("%lg", &i);
	Stack::push(i);
}

void CPU::push(Data_t *x) {
	Stack::push(*x);
}

void CPU::pop(Data_t *x) {
	*x = Stack::pop();
}