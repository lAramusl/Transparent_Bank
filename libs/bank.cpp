#include "headers/bank.h"
#include <iostream>

Bank::Bank(std::size_t size, Bank_cell* ptr)
  : size(size), arr(ptr) 
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
	arr[fromCell].credit(amount);
	arr[toCell].deposit(amount);
}

void Bank::credit(long amount)
{
	for(std::size_t i = 0; i < size; ++i)
	{
		arr[i].credit(amount);
	}
}

void Bank::deposit(long amount)
{
	for(std::size_t i = 0; i < size; ++i)
	{
		arr[i].deposit(amount);
	}
}
