#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cerrno>
#include <semaphore.h>
#include "libs/headers/bank.h"

int main(int argc, char** argv)
{
  if(argc != 2)
  {
    std::cerr << "Invalid Arguments\n";
    exit(EXIT_FAILURE);
  }
  
	unsigned long numberOfCells = std::stoul(argv[1]);
	
	//creating shared mem
	const char* sharedMem = "/Bank";

	int sharedFd = shm_open(sharedMem, O_CREAT|O_RDWR|O_TRUNC, 0666);
	if(sharedFd == -1)
	{
		std::cerr << "shm_open: failure at creating\n";
		exit(errno);
	}

	std::size_t sizeOfSharedMem = numberOfCells*sizeof(Bank_cell);

	int trunc = ftruncate(sharedFd, sizeOfSharedMem);
	if(trunc == -1)
	{
		std::cerr << "truncate: could not truncate\n";
		exit(errno);
	}
	
	//filling the shared mem with bank cells
	void* ptr = mmap(nullptr,
		       	sizeOfSharedMem,
		       	PROT_READ|PROT_WRITE,
		       	MAP_SHARED,
			sharedFd,
			0);
	if(ptr == MAP_FAILED)
	{
		std::cerr << "mmap: could not map\n";
		exit(errno);
	}
	
	Bank pBank(numberOfCells, (Bank_cell*)(ptr));

	for(std::size_t i = 0; i < numberOfCells; ++i)
	{
		pBank[i] = Bank_cell(i);//one bilion
	}

  //unmapping
  int munm = munmap(ptr, sizeOfSharedMem);
  if(munm == -1)
  {
    std::cerr << "munmap: could not unmap\n";
    exit(errno);
  }
  
  //closing shared memory's file descriptor
  int shmClose = close(sharedFd);
  if(shmClose == -1)
  {
    std::cerr << "close: could not close\n";
    exit(errno);
  }
  
	return 0;	
}
