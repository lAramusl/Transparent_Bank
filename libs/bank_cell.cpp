#include "headers/bank_cell.h"
#include <string>
#include <fcntl.h>

Bank_cell::Bank_cell(std::size_t index) : 
  index(index),
	minAmount(0), 
	maxAmount(1000000000),
	currentAmount(0),
	frozenState(false)
{
	semf = sem_open(("/cellSem"+std::to_string(index)).data(),
  O_CREAT|O_RDWR|O_TRUNC,
  0666, 
  1);
	std::cout << "Bank_cell " << index << ": constructed successfully\n";
}

void Bank_cell::freeze()
{
	frozenState = true;
}

void Bank_cell::unfreeze()
{
	frozenState = false;
}

void Bank_cell::deposit(long Amount)
{
	currentAmount += Amount;
}

void Bank_cell::credit(long Amount)
{
	currentAmount -= Amount;
}

const long Bank_cell::getMax()
{
	return maxAmount;
}

const long Bank_cell::getMin()
{
	return minAmount;
}

const long Bank_cell::getCurrent()
{
	return currentAmount;
}

bool Bank_cell::isFrozen()
{
	return frozenState;
}

void Bank_cell::post()
{
  if(sem_post(semf) == -1)
  {
    std::cerr << "cell number:" << index << "semaphore error\n";
    exit(errno);
  }
}