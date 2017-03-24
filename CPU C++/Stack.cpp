#include "Stack.h"                                           

Stack::Stack(size_t cap) :
	data_		((StackData_t *)calloc(cap, sizeof(StackData_t))),
	capacity_	(cap),
	size_		(0)
{
	for (int i = 0; i < capacity_; i++) {
		i[data_] = -666;
	}
}

Stack::~Stack()
{
	for (int i = 0; i < capacity_; i++) {
		data_[i] = -666;
	}
	*data_ = NULL;
	capacity_ = -666;
	size_ = -666;
}

void Stack::push(double value) {
	if (size_ >= (capacity_ - 2)) {
		capacity_ = capacity_ + 10;
		data_ = (StackData_t *)realloc(data_, capacity_ * sizeof(StackData_t));
		if (data_ == NULL) printf("Can't allocate memory\n");
		for (int i = (capacity_ - 10); i < capacity_;i++) {
			data_[i] = -666;
		}
	}
	data_[size_++] = value;
}

bool Stack::Ok() const {
	return ((data_ && capacity_ && size_ < capacity_) || (!data_ && !capacity_ && !size_));
}

//#define DUMP(file,Stack_name); {						\
//		assert(strlen(#Stack_name) < MAX_LENGTH);       \
//		Stack_name.dump(file,#Stack_name);              \
//	}

void Stack::dump(FILE* stream, char name[MAX_LENGTH]) const {
	fprintf(stream, "Stack '%s' ", name);
	fprintf(stream, Ok() ? "(Ok)\n" : "(NotOk)\n");
	fprintf(stream, "    {\n");
	fprintf(stream, "    capacity_ = %i\n", capacity_);
	fprintf(stream, "    size_ = %i\n", size_);
	fprintf(stream, "    data_[%p]\n", data_);
	fprintf(stream, "\t{\n");
	for (int i = 0; i < size_; i++) {
		fprintf(stream, "\t* [%i] %lg\n", i, data_[i]);
	}
	for (int i = size_; i < capacity_; i++) {
		if (data_[i] == -666) {
			fprintf(stream, "\t  [%i] %lg  POISON!\n", i, data_[i]);
		}
		else fprintf(stream, "\t  [%i] %lg\n", i, data_[i]);
	}
	fprintf(stream, "\t}\n");
	fprintf(stream, "    }\n");
}

bool Stack::empty() const {
	return !size_;
}

int Stack::size() const {
	return size_;
}

int Stack::capacity() const {
	return capacity_;
}

void Stack::clear() {
	size_ = 0;
}