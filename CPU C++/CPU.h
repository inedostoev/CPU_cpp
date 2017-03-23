#pragma once
#include "Stack.h"

typedef double Data_t;

class CPU: public Stack
{
public:
	CPU::CPU(int cap) : Stack(cap) {
		Data_t ax_ = -666;
		Data_t bx_ = -666;
		Data_t cx_ = -666;
		Data_t dx_ = -666;
		commands_ = (int *)malloc(10 * sizeof(int));
		ip_ = 0;
		max_ip = 1;
	}
	~CPU();
	void		out();
	void		in();
	void		push(Data_t *x);
	void		pop(Data_t *x);
	void		convector(FILE *stream);
private:
	void		resize_mas(int max);
	void		add();
	void		sub();
	void		mal();
	void		div();
	void		hlt(int max);
	Data_t		ax_;
	Data_t		bx_;
	Data_t		cx_;
	Data_t		dx_;
	int			*commands_;
	int			ip_;
	int			max_ip;
};

