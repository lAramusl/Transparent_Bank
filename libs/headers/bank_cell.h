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

	void deposit(long Amount);
	void credit(long Amount);
	
	const long getMax();
	const long getMin();
	const long getCurrent();
	bool isFrozen();

  void post();
  void wait();

private:
  std::size_t index;
	long minAmount;
	long maxAmount;
	long currentAmount;
	bool frozenState;
  sem_t* semf;

};

#endif
