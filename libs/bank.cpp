#include "headers/bank.h"
#include <iostream>

Bank::Bank(std::size_t size, Bank_cell* ptr)
  : _size(size), arr(ptr) 
{
  std::cout << "Bank: Constructed successfuly\n";
}

Bank_cell& Bank::operator[](std::size_t cellNum)
{
	return arr[cellNum];
}

const Bank_cell& Bank::operator[](std::size_t cellNum) const
{
	return arr[cellNum];
}

void Bank::transfer(std::size_t fromCell, std::size_t toCell, long amount)
{
	if(arr[fromCell].isFrozen())
	{
		std::cout << "Cell " << arr[fromCell].getIndex() << " is frozen\ntransfer is impossible\n";
		return;
	}
	else if(arr[toCell].isFrozen())
	{
		std::cout << "Cell " << arr[toCell].getIndex() << " is frozen\ntransfer is impossible\n";
		return;
	}
	if(arr[fromCell].credit(amount) == 1)
	{
		return;
	}
	if(arr[toCell].deposit(amount) == 1)
	{
		arr[fromCell].deposit(amount);
		return;
	}
}
