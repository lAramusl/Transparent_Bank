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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <atomic>
#include <sstream>
#include <functional>

static std::atomic<std::size_t> clientNum(0);

std::stringstream BankAction(std::stringstream& in, Bank& bnk)
{
    std::stringstream out;
	std::cout << "BankAction:\n";
	static const std::vector<std::string> commands = 
	{ "getmax", "getmin", "getballance", 
	  "transfer", 
	  "freeze", "unfreeze",
	  "creditfrom", "depositto",
	  "credit", "deposit",
	  "changemax", "changemin",
	  "info"};

    std::string cmd;
    in >> cmd;
	auto it = std::find(commands.begin(), commands.end(), cmd);

	const std::string acomplished = "acomplished\n";

	if(it == commands.end())
	{
		out << "Invalid operation\n";
		return out;
	}
	std::size_t index = it - commands.begin();

	switch(index)
	{
		case 0://get_max
		{
			std::size_t cellNum = 0;
			in >> cellNum;
			if(cellNum >= bnk.size())
			{
				out << "Invalid bank cell number\n";
			}
            else
            {
			    bnk[cellNum].wait();
			    out << "The max balance of cell " << cellNum << " is " << bnk[cellNum].getMax() << "\n";
			    bnk[cellNum].post();
            }
			break;
		}
		case 1://get_min
		{
			std::size_t cellNum = 0;
			in >> cellNum;
			if(cellNum >= bnk.size())
			{
				out << "Invalid bank cell number\n";
			}
            else
            {
			    bnk[cellNum].wait();
			    out << "The min balance of cell " << cellNum << " is " << bnk[cellNum].getMin() << "\n";
			    bnk[cellNum].post();
            }
			break;
		}
		case 2://get_ballance
		{
			std::size_t cellNum = 0;
			in >> cellNum;
			if(cellNum >= bnk.size())
			{
				out << "Invalid bank cell number\n";
			}
            else
            {
			    bnk[cellNum].wait();
			    out << "The current balance of cell " << cellNum << " is " << bnk[cellNum].getCurrent() << "\n";
			    bnk[cellNum].post();
            }
			break;
		}
		case 3://transfer
		{
			std::size_t from = 0;
			std::size_t to = 0;
			std::size_t amount = 0;
			in >> from >> to >> amount;
			if(from >= bnk.size() || to >= bnk.size())
			{
				out << "Invalid bank cell number\n";
			}
            else
            {
			    bnk[from].wait();
			    bnk[to].wait();
			    bnk.transfer(from, to, amount);
			    bnk[from].post();
			    bnk[to].post();
            }
			break;
		}
		case 4://freeze
		{
			std::size_t cellNum = 0;
			in >> cellNum;
			if(cellNum >= bnk.size())
			{
				out << "Invalid bank cell number\n";
			}
            else
            {
			    bnk[cellNum].wait();
			    bnk[cellNum].freeze();
			    bnk[cellNum].post();
            }
			out << acomplished;
			break;
		}
		case 5://unfreeze
		{
			std::size_t cellNum = 0;
			in >> cellNum;
			if(cellNum >= bnk.size())
			{
				out << "Invalid bank cell number\n";
			}
            else
            {
			    bnk[cellNum].wait();
			    bnk[cellNum].unfreeze();
			    bnk[cellNum].post();
            }
			out << acomplished;
			break;
		}
		case 6://credit_from
		{
			std::size_t cellNum = 0;
			std::size_t amount = 0;
			in >> cellNum >> amount;
			if(cellNum >= bnk.size())
			{
				out << "Invalid bank cell number\n";
			}
            else
            {
			    bnk[cellNum].wait();
			    if(bnk[cellNum].credit(amount) == 0)
			    {
			    	out << acomplished;
			    }
                else
                {
                    out << "the cell does not contain such amount\n";
                }
			    bnk[cellNum].post();
            }
			break;
		}
		case 7://deposit_to
		{
			std::size_t cellNum = 0;
			std::size_t amount = 0;
			in >> cellNum >> amount;
			if(cellNum >= bnk.size())
			{
				out << "Invalid bank cell number\n";
			}
            else
            {
			    bnk[cellNum].wait();
			    if(bnk[cellNum].deposit(amount) == 0)
			    {
			    	out << acomplished;
			    }
                else
                {
                    out << "this amount wont fit into cell\n";
                }
			    bnk[cellNum].post();
            }
			break;
		}
		case 8://credit
		{
			std::size_t amount = 0;
			in >> amount;
			for(std::size_t i = 0; i < bnk.size(); ++i)
			{
				bnk[i].wait();
				if(bnk[i].credit(amount) == 0)
				{
					out << "Cell " << i << ": " << acomplished;
				}
                else
                {
                    out << "Cell " << i << ": " << "cannot credit\n";
                }
				bnk[i].post();
			}
			break;
		}
		case 9://deposit
		{
			std::size_t amount = 0;
			in >> amount;
			for(std::size_t i = 0; i < bnk.size(); ++i)
			{
				bnk[i].wait();
				if(bnk[i].deposit(amount) == 0)
				{
					out << "Cell " << i << ": " << acomplished;
				}
                else
                {
                    out << "Cell " << i << ": " << "cannot deposit\n";
                }
				bnk[i].post();
			}
			break;
		}
		case 10://change_max
		{
			std::size_t cellNum = 0;
			long newMax = 0;
			in >> cellNum >> newMax;
			if(cellNum >= bnk.size())
			{
				out << "Invalid bank cell number\n";
			}
            else
            {
			    bnk[cellNum].wait();
			    if(bnk[cellNum].changeMax(newMax) == 0)
			    {
			    	out << acomplished;
			    }
                else
                {
                    out << "invalid amount\n";
                }
			    bnk[cellNum].post();
            }
		}
		case 11://change_min
		{
			std::size_t cellNum = 0;
			long newMin = 0;
			in >> cellNum >> newMin;
			if(cellNum >= bnk.size())
			{
				out << "Invalid bank cell number\n";
			}
            else
            {
			    bnk[cellNum].wait();
			    if(bnk[cellNum].changeMin(newMin) == 0)
			    {
			    	out << acomplished;
			    }
                else
                {
                    out << "invalid amount\n";
                }
			    bnk[cellNum].post();
            }
		}
		case 12://info
		{
			std::size_t cellNum = 0;
			in >> cellNum;
			if(cellNum >= bnk.size())
			{
				out << "Invalid bank cell number\n";
			}
            else
            {
			    bnk[cellNum].wait();
			    std::string state = "is not frozen";
			    if(bnk[cellNum].isFrozen())
			    {
			    	state = "frozen";
			    }
			        out << "Info of Cell " << cellNum 
			    	<< ":\nmin: " << bnk[cellNum].getMin()
			    	<< "\nballance: " << bnk[cellNum].getCurrent()
			    	<< "\nmax: " << bnk[cellNum].getMax()
			    	<< "\nstate: " << state << "\n";
			    bnk[cellNum].post();
            }
		}
	}

    return out;
}

void clientHandler(int client_socket, std::string clientIP, Bank& bnk)
{
  // Receive messages from client
	std::cout << "clientHandler:\n\n";
	const int bufferSize = 4096;

	while(true)
	{
		//std::stringstream ss;
		std::stringstream cmd;
		volatile std::string buffer;
		buffer.resize(bufferSize);

		int rs = recv(client_socket, buffer.data(), bufferSize, 0);
		if (rs == -1) 
		{
			perror("client socket connection error");
			close(client_socket);
			exit(1);
		}
		buffer[rs] = '\0';
		buffer.shrink_to_fit();
		std::cout << buffer;
		if(buffer == "disconnect")
		{
			break;
		}
		//ss << "From client " << clientIP << " number: " << buffer << '\n';
		cmd << buffer;
		//std::cout << ss.str();

		std::stringstream result = BankAction(cmd, bnk);
		std::string res = result.str();
		std::cout << res << std::endl;
		int snd = send(client_socket, res.c_str(), res.size(), 0);
		if( snd == -1)
		{
			perror("client message send error");
			exit(1);
		}
	
	}
	//ss << "client is disconnecting\n";//вот тут когда disconnect получает то падает в беск. цикл
	close(client_socket);
	clientNum.fetch_sub(1);
	//std::cout << ss.str();

	return;
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

  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket == -1) {
      perror("socket creation error");
      exit(errno);
  }

  // create an endpoint
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(9888);

  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) 
  {
      perror("bind failed");
      exit(errno);
  }

  // Listen for incoming connections
  if (listen(server_socket, 1024) < 0) {
    perror("listen failed");
    exit(errno);
  }
  std::cout << "Waiting for connection\n";

	int client_socket;
      struct sockaddr_in client_address;
      unsigned int client_addr_len = sizeof(client_address);

      // Accept incoming connection
      if ((client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len)) < 0) {
          perror("accept failed");
          exit(errno);
      }
    //std::vector<std::thread> threadPool;
	//while(true)
	//{
      

      		std::cout<< " Connected client with address: " << inet_ntoa(client_address.sin_addr) << "\n";

           clientHandler(client_socket, inet_ntoa(client_address.sin_addr), std::ref<Bank>(bnk));

            //clientNum.fetch_add(1);
        
	//}

    // for (auto& thread : threadPool)
    // {
    //     if (thread.joinable())
    //     {
    //         thread.join();
    //     }
    // }

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

	std::cout << "Thank you for using our Bank\n";

	return 0;
}
