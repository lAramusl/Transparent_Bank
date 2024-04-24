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
	
	void transfer(std::size_t fromCell,
		      std::size_t toCell,
			long amount);

	std::size_t size() const { return _size; }

private:
	std::size_t _size;
	Bank_cell* arr;
};


#endif
