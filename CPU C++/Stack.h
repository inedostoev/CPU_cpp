#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef double StackData_t;
enum { MAX_LENGTH = 256 };  

class Stack
{
public:
	Stack(size_t cap);
	~Stack();
	void        push(double value);
#define func(func, oper);	StackData_t func() {	                                \
	if (size_ == 0)																	\
		printf("Error: stack underflow\n");  										\
	else return data_[oper];														\
} 
				func(pop, --size_);
				func(peek, size_ - 1);
#undef func
	bool        Ok() const;
	void        dump(FILE* stream, char Stack_name[MAX_LENGTH]) const;
	bool        empty() const;
	int         size() const;
	int         capacity() const;
	void        clear();
private:
	StackData_t *data_;
	int			capacity_;
	int			size_;
};	