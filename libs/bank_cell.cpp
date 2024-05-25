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
  O_CREAT|O_RDWR,
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

int Bank_cell::changeMax(long amount)
{
	if(isFrozen())
	{
		std::cout << "Cell " << index << "  is frozen\n";
		return 3;
	}
	if(amount < currentAmount)
	{
		currentAmount = amount;
	}
	if(amount <= minAmount)
	{
		std::cout << "Invalid Amount\n";
		return 2;
	}
	maxAmount = amount;
	return 0;
}

int Bank_cell::changeMin(long amount)
{
	if(isFrozen())
	{
		std::cout << "Cell " << index << " is frozen\n";
		return 3;
	}
	if(amount > currentAmount)
	{
		std::cout << "Cell " << index << ": cannot change, add some amount first\n";
		return 1;
	}
	if(amount >= maxAmount)
	{
		std::cout << "Invalid Amount\n";
		return 2;
	}
	minAmount = amount;
	return 0;
}

int Bank_cell::deposit(long amount)
{
	if(isFrozen())
	{
		std::cout << "Cell " << index << " is frozen\n";
		return 2;
	}
	if(currentAmount + amount > maxAmount)
	{
		std::cout << "Cell " << index << ": this amount wont fit into cell\n";
		return 1;
	}
	currentAmount += amount;
	return 0;
}

int Bank_cell::credit(long amount)
{
	if(isFrozen())
	{
		std::cout << "Cell " << index << "is frozen\n";
		return 2;
	}
	if(currentAmount - amount < minAmount)
	{
		std::cout << "Cell " << index << ": the cell does not contain such amount\n";
		return 1;
	}
	currentAmount -= amount;
	return 0;
}

const long Bank_cell::getMax() const
{
	return maxAmount;
}

const long Bank_cell::getMin() const
{
	return minAmount;
}

const long Bank_cell::getCurrent() const
{
	return currentAmount;
}

const std::size_t Bank_cell::getIndex() const
{
	return index;
}

bool Bank_cell::isFrozen() const
{
	return frozenState;
}

void Bank_cell::openSem()
{
	semf = sem_open(("/cellSem"+std::to_string(index)).data(), O_RDWR, 0666, 1);
}

void Bank_cell::closeSem()
{
	if(sem_close(semf) == -1)
	{
		std::cerr << "sem_close of " << index << ": could not close\n";
		exit(errno);
	}
}

void Bank_cell::wait()
{
  if(sem_wait(semf) == -1)
  {
    std::cerr << "cell number:" << index << "semaphore error\n";
    exit(errno);
  }
}

void Bank_cell::post()
{
  if(sem_post(semf) == -1)
  {
    std::cerr << "cell number:" << index << "semaphore error\n";
    exit(errno);
  }
}