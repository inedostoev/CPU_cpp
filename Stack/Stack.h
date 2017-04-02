#ifndef STACK_H_UNCLUDED
#define STACK_H_UNCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef double StackData_t;
enum { MAX_LENGTH = 256 };  

class Stack
{
public:
                            Stack();
	                        ~Stack();
	void                    push(double value);
#define FUNC(func, oper);	StackData_t func() {            \
	if (size_ == 0) {										\
		printf("Error: stack underflow\n");          		\
        assert(size_ > 0);                                  \
    }                                                       \
	else return data_[oper];								\
} 
				            FUNC(pop, --size_);
				            FUNC(peek, size_ - 1);
#undef FUNC
	bool                    Ok() const;
	void                    dump(FILE* stream, char Stack_name[MAX_LENGTH]) const;
	bool                    empty() const;
	int                     size() const;
	int                     capacity() const;
	void                    clear();
private:
    static const size_t     init_size = 64;
	StackData_t             data_[init_size];
	int			            capacity_;
	int			            size_;
};
#endif
