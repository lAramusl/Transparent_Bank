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
	  "freeze", "unfreeze",
	  "credit_from", "deposit_to",
	  "credit", "deposit",
	  "change_max", "change_min"};

	auto it = std::find(commands.begin(), commands.end(), cmd);

	const std::string acomplished = "acomplished\n";

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
			if(cellNum >= bnk.size())
			{
				std::cout << "Invalid bank cell number\n";
			}
			bnk[cellNum].wait();
			std::cout << "The max balance of cell " << cellNum << " is " << bnk[cellNum].getMax() << std::endl;
			bnk[cellNum].post();
			break;
		}
		case 1://get_min
		{
			std::size_t cellNum = 0;
			std::cin >> cellNum;
			if(cellNum >= bnk.size())
			{
				std::cout << "Invalid bank cell number\n";
			}
			bnk[cellNum].wait();
			std::cout << "The min balance of cell " << cellNum << " is " << bnk[cellNum].getMin() << std::endl;
			bnk[cellNum].post();
			break;
		}
		case 2://get_ballance
		{
			std::size_t cellNum = 0;
			std::cin >> cellNum;
			if(cellNum >= bnk.size())
			{
				std::cout << "Invalid bank cell number\n";
			}
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
			if(from >= bnk.size() || to >= bnk.size())
			{
				std::cout << "Invalid bank cell number\n";
			}
			std::cin >> from >> to >> amount;
			bnk[from].wait();
			bnk[to].wait();
			bnk.transfer(from, to, amount);
			bnk[from].post();
			bnk[to].post();
			break;
		}
		case 4://freeze
		{
			std::size_t cellNum = 0;
			std::cin >> cellNum;
			if(cellNum >= bnk.size())
			{
				std::cout << "Invalid bank cell number\n";
			}
			bnk[cellNum].wait();
			bnk[cellNum].freeze();
			bnk[cellNum].post();
			std::cout << acomplished;
			break;
		}
		case 5://unfreeze
		{
			std::size_t cellNum = 0;
			std::cin >> cellNum;
			if(cellNum >= bnk.size())
			{
				std::cout << "Invalid bank cell number\n";
			}
			bnk[cellNum].wait();
			bnk[cellNum].unfreeze();
			bnk[cellNum].post();
			std::cout << acomplished;
			break;
		}
		case 6://credit_from
		{
			std::size_t cellNum = 0;
			std::size_t amount = 0;
			std::cin >> cellNum >> amount;
			if(cellNum >= bnk.size())
			{
				std::cout << "Invalid bank cell number\n";
			}
			bnk[cellNum].wait();
			if(bnk[cellNum].credit(amount) == 0)
			{
				std::cout << acomplished;
			}
			bnk[cellNum].post();
			break;
		}
		case 7://deposit_to
		{
			std::size_t cellNum = 0;
			std::size_t amount = 0;
			std::cin >> cellNum >> amount;
			if(cellNum >= bnk.size())
			{
				std::cout << "Invalid bank cell number\n";
			}
			bnk[cellNum].wait();
			if(bnk[cellNum].deposit(amount) == 0)
			{
				std::cout << acomplished;
			}
			bnk[cellNum].post();
			break;
		}
		case 8://credit
		{
			std::size_t amount = 0;
			std::cin >> amount;
			for(std::size_t i = 0; i < bnk.size(); ++i)
			{
				bnk[i].wait();
				if(bnk[i].credit(amount) == 0)
				{
					std::cout << "Cell " << i << ": " << acomplished;
				}
				bnk[i].post();
			}
			break;
		}
		case 9://deposit
		{
			std::size_t amount = 0;
			std::cin >> amount;
			for(std::size_t i = 0; i < bnk.size(); ++i)
			{
				bnk[i].wait();
				if(bnk[i].deposit(amount) == 0)
				{
					std::cout << "Cell " << i << ": " << acomplished;
				}
				bnk[i].post();
			}
			break;
		}
		case 10://change_max
		{
			std::size_t cellNum = 0;
			long newMax = 0;
			std::cin >> cellNum >> newMax;
			if(cellNum >= bnk.size())
			{
				std::cout << "Invalid bank cell number\n";
			}
			bnk[cellNum].wait();
			if(bnk[cellNum].changeMax(newMax) == 0)
			{
				std::cout << acomplished;
			}
			bnk[cellNum].post();
		}
		case 11://change_min
		{
			std::size_t cellNum = 0;
			long newMin = 0;
			std::cin >> cellNum >> newMin;
			if(cellNum >= bnk.size())
			{
				std::cout << "Invalid bank cell number\n";
			}
			bnk[cellNum].wait();
			if(bnk[cellNum].changeMin(newMin) == 0)
			{
				std::cout << acomplished;
			}
			bnk[cellNum].post();
		}
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

	for(std::size_t i = 0; i < bnk.size(); ++i)
	{
		bnk[i].openSem();
	}

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

	//std::cout << bnk[0].getMax() <<std::endl;

	for(std::size_t i = 0; i < bnk.size(); ++i)
	{
		bnk[i].closeSem();
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
