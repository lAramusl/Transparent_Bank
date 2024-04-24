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

void BankAction(const std::string& cmd, Bank*& bnk)
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
		case 0:
		{
			std::size_t cellNum = 0;
			std::cin >> cellNum;
                        std::cout << "The max balance of cell " << cellNum << " is " << bnk->getMaxBalance(cellNum) << std::endl;
		}
		case 1:
		{
			std::size_t cellNum = 0;
                        std::cin >> cellNum;
                        std::cout << "The min balance of cell " << cellNum << " is " << bnk->getMinBalance(cellNum) << std::endl;
		}
		case 2:
		{
			std::size_t cellNum = 0;
                        std::cin >> cellNum;
                        std::cout << "The current balance of cell " << cellNum << " is " << bnk->getBalance(cellNum) << std::endl;
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

}
