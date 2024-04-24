#ifndef BANK_CELLH
#define BANK_CELLH
#include <iostream>
#include <cstddef>
#include <semaphore.h>

class Bank_cell
{
public:
	Bank_cell(std::size_t index);

	void freeze();
	void unfreeze();

	int deposit(long Amount);
	int credit(long Amount);
	
	int changeMax(long amount);
	int changeMin(long amount);

	const long getMax() const;
	const long getMin() const;
	const long getCurrent() const;
	const std::size_t getIndex() const;
	bool isFrozen() const;

  	void wait();
  	void post();

private:
  	std::size_t index;
	long minAmount;
	long maxAmount;
	long currentAmount;
	bool frozenState;
  	sem_t* semf;

};

#endif
