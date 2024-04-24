#include <iostream>
#include "libs/headers/bank.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <string>
#include <vector>
#include <algorithm>

void BankAction(const std::string& cmd, Bank& bnk)
{
	static const std::vector<std::string> commands = 
	{ "get_max", "get_min", "get_ballance", 
	  "transfer", 
	  "freeze", "unfreeze", "isfrozen",
	  "credit_from", "deposit_to",
	  "credit", "deposit"};

	auto it = std::find(commands.begin(), commands.end(), cmd);

	if(it == commands.end())
	{
		std::cout << "Invalid operation\n";
		return;
	}
	std::size_t index = it - commands.begin();

	switch(index)
	{
		case 0://get_max
		{
			std::size_t cellNum = 0;
			std::cin >> cellNum;

			std::cout << "The max balance of cell " << cellNum << " is " << bnk[cellNum].getMax() << std::endl;

			break;
		}
		case 1://get_min
		{
			std::size_t cellNum = 0;
			std::cin >> cellNum;

			bnk[cellNum].wait();
			std::cout << "The min balance of cell " << cellNum << " is " << bnk[cellNum].getMin() << std::endl;
			bnk[cellNum].post();
			break;
		}
		case 2://get_ballance
		{
			std::size_t cellNum = 0;
			std::cin >> cellNum;

			bnk[cellNum].wait();
			std::cout << "The current balance of cell " << cellNum << " is " << bnk[cellNum].getCurrent() << std::endl;
			bnk[cellNum].post();
			break;
		}
		case 3://transfer
		{
			std::size_t from = 0;
			std::size_t to = 0;
			std::size_t amount = 0;

			std::cin >> from >> to >> amount;
			bnk[from].wait();
			bnk[to].wait();
			bnk.transfer(from, to, amount);
			bnk[from].post();
			bnk[to].post();
			break;
		}
		case 4://freeze

	}

}

int main(int argc,char** argv)
{
  if(argc != 2)
  {
    std::cerr << "Invalid Arguments\n";
    exit(EXIT_FAILURE);
  }
  
	const char* shmName = "/Bank"; 
	std::size_t numberOfCells = std::stoull(argv[1]);
	std::size_t shmSize = numberOfCells*sizeof(Bank_cell);

	int shmFd = shm_open(shmName, O_RDWR, 0666);

	void* bankptr = mmap(nullptr, shmSize, PROT_READ|PROT_WRITE, MAP_SHARED, shmFd, 0);	

	Bank bnk(numberOfCells,(Bank_cell*)bankptr);

	while(true)
	{
		std::string cmd;
		std::cin >> cmd;
		if(cmd == "exit")
		{
			break;
		}
		BankAction(cmd, bnk);
	}

	if(munmap(bankptr,shmSize) == -1)
	{
		std::cerr << "munmap: error at unammping\n";
		exit(errno);
	}

	if(close(shmFd) == -1)
	{
		std::cerr << "close: error at closing shmFd\n";
		exit(errno);
	}

	std::cout << "This bank is sponsored by 1xBET\n";

	return 0;
}
