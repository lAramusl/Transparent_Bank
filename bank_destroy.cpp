#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cerrno>
#include <semaphore.h>
#include "libs/headers/bank.h"
#include <string>

int main(int argc, char** argv)
{
  const char* sharedMem = "/Bank";

 if(argc != 2)
  {
    std::cerr << "Invalid Arguments\n";
    exit(EXIT_FAILURE);
  }

  unsigned long numberOfCells = std::stoul(argv[1]);
  for(std::size_t i = 0; i < numberOfCells; ++i)
    {
      std::string cellSemName = "/cellSem";
      cellSemName += std::to_string(i);
      //std::cout <<"trying to unlink " << cellSemName << std::endl;
      if(sem_unlink(cellSemName.data()) == -1)
      {
        std::cerr << "sem_unlink: could not unlink\n";
        exit(errno);
      }
    }
  std::cout << "Cell Semaphores deleted successfully!\n";
  
  if(shm_unlink(sharedMem) == -1)
  {
    std::cerr << "shm_unlink: could not unlink";
    exit(errno);
  }
  std::cout << "Shared Memory deleted successfully!\n";

  return 0;
}