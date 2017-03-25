#include "Stack.h"                                           

Stack::Stack() :
	capacity_	(init_size),
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
	capacity_ = -666;
	size_ = -666;
}

void Stack::push(double value) {
	if (capacity_ == size_) {
		assert(!"Error: stack overflow");
	}
	data_[size_++] = value;
}

bool Stack::Ok() const {
	return ((data_ && capacity_ && size_ < capacity_) || (!data_ && !capacity_ && !size_));
}

void Stack::dump(FILE* stream, char name[MAX_LENGTH]) const {
	fprintf(stream, "Stack '%s' ", name);
	fprintf(stream, Ok() ? "(Ok)\n" : "(NotOk)\n");
	fprintf(stream, "    {\n");
	fprintf(stream, "    capacity_ = %i\n", capacity_);
	fprintf(stream, "    size_ = %i\n", size_);
	fprintf(stream, "    data_[%p]\n", data_);
	fprintf(stream, "\t{\n");
	for (int i = 0; i < size_; i++) {
		fprintf(stream, "\t* [%4i] %lg\n", i, data_[i]);
	}
	for (int i = size_; i < capacity_; i++) {
		if (data_[i] + 666 < 0.001) {
			fprintf(stream, "\t  [%2i] %lg  POISON!\n", i, data_[i]);
		}
		else fprintf(stream, "\t  [%2i] %lg\n", i, data_[i]);
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
