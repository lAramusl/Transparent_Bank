#ifndef BANKH
#define BANKH
#include "bank_cell.h"
#include <iostream>
#include <vector>
#include <cstddef>

class Bank
{
public:

	Bank(std::size_t size, Bank_cell* ptr);

	Bank_cell& operator[](std::size_t cellNum);

	const Bank_cell& operator[](std::size_t cellNum) const;
	
	long getMaxBalance(std::size_t cellNum);
	long getMinBalance(std::size_t cellNum);
	long getBalance(std::size_t cellNum);

	void transfer(std::size_t fromCell,
		      std::size_t toCell,
			long amount);

	void freezeCell(std::size_t cellNum);
	void unFreezeCell(std::size_t cellNum);

	bool isFrozen(std::size_t cellNum);

	void creditFrom(std::size_t cellNum, long amount);
	void depositTo(std::size_t cellNum, long amount);

	void credit(long amount);
	void deposit(long amount);

private:
	std::size_t size;
	Bank_cell* arr;
};


#endif
