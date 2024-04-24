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

long Bank::getMaxBalance(std::size_t cellNum)
{
	return arr[cellNum].getMax();
}

long Bank::getMinBalance(std::size_t cellNum)
{
	return arr[cellNum].getMin();
}

long Bank::getBalance(std::size_t cellNum)
{
	return arr[cellNum].getCurrent();
}

void Bank::transfer(std::size_t fromCell, std::size_t toCell, long amount)
{
	if(arr[fromCell].getCurrent() - amount < arr[fromCell].getMin())
	{
		std::cout << "Not enough amount of money, transfer impossible\n";
		return;
	}
	if(arr[toCell].getCurrent() + amount > arr[toCell].getMax())
	{
		std::cout << "Too big amount of money, transfer impossible\n";
		return;
	}
	arr[fromCell].credit(amount);
	arr[toCell].deposit(amount);
}

void Bank::freezeCell(std::size_t cellNum)
{
	arr[cellNum].freeze();
}

void Bank::unFreezeCell(std::size_t cellNum)
{
	arr[cellNum].freeze();
}

bool Bank::isFrozen(std::size_t cellNum)
{
	return arr[cellNum].isFrozen();
}

void Bank::creditFrom(std::size_t cellNum, long amount)
{
	if(arr[cellNum].getCurrent() - amount < arr[cellNum].getMin())
	{
		std::cout <<"Cell Number: " << cellNum
			<<"\nCannot credit, no enough amount of money\n";
		return;
	}
	arr[cellNum].credit(amount);
}

void Bank::depositTo(std::size_t cellNum, long amount)
{
	if(arr[cellNum].getCurrent() + amount > arr[cellNum].getMax())
	{
		std::cout <<"Cell Number: "<< cellNum
			<< "\nCannot deposit, not enough space for money\n";
		return;
	}
	arr[cellNum].credit(amount);
}

void Bank::credit(long amount)
{
	for(std::size_t i = 0; i < size; ++i)
	{
		creditFrom(i, amount);
	}
}

void Bank::deposit(long amount)
{
	for(std::size_t i = 0; i < size; ++i)
	{
		depositTo(i, amount);
	}
}
