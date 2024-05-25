.PHONY: clean

CXXFLAGS = -Wall -Werror -pthread -lrt -o

DEBUGCXXFLAGS = -g -Wall -Werror -pthread -lrt -fsanitize=address -o

OBJECT = -c -pthread -lrt

DEBUGOBJECT = -g $(OBJECT)

BANK = bank.o bank_cell.o

DEBUGBANK = bankDebug.o bank_cellDebug.o 

 
release: $(BANK) init.o client.o destroy.o
	g++ bank_init.o $(BANK) $(CXXFLAGS) init
	g++ client.o $(BANK) $(CXXFLAGS) client
	g++ bank_destroy.o $(BANK) $(CXXFLAGS) destroy

debug: $(DEBUGBANK) initDebug.o clientDebug.o destroyDebug.o
	g++ bank_init.o $(DEBUGBANK) $(DEBUGCXXFLAGS) init
	g++ client.o $(DEBUGBANK) $(DEBUGCXXFLAGS) client
	g++ bank_destroy.o $(DEBUGBANK) $(DEBUGCXXFLAGS) destroy

releaseServer: release $(BANK) server.o server_client.o 
	g++ server.o $(BANK)  $(CXXFLAGS) server
	g++ server_client.o $(BANK) $(CXXFLAGS) server_client

debugServer: debug $(DEBUGBANK) serverDebug.o server_clientDebug.o 
	g++ server.o $(DEBUGBANK) $(DEBUGCXXFLAGS) server
	g++ server_client.o $(DEBUGBANK) $(DEBUGCXXFLAGS) server_client

clean:
	rm -f init client destroy server server_client
	rm *.o

bank_cell.o: libs/bank_cell.cpp libs/headers/bank_cell.h
	g++ libs/bank_cell.cpp $(OBJECT) 

bank.o: libs/bank.cpp libs/headers/bank.h
	g++ libs/bank.cpp $(OBJECT)

init.o: bank_init.cpp
	g++ bank_init.cpp $(OBJECT)

client.o: client.cpp
	g++ client.cpp $(OBJECT)

server.o: server.cpp 
	g++ server.cpp $(OBJECT) 

server_client.o: server_client.cpp
	g++ server_client.cpp $(OBJECT)

destroy.o: bank_destroy.cpp
	g++ bank_destroy.cpp $(OBJECT)

bankDebug.o: libs/bank.cpp libs/headers/bank.h
	g++ libs/bank.cpp $(DEBUGOBJECT) -o bankDebug.o

bank_cellDebug.o: libs/bank_cell.cpp libs/headers/bank_cell.h
	g++ libs/bank_cell.cpp $(DEBUGOBJECT) -o bank_cellDebug.o

initDebug.o: bank_init.cpp
	g++ bank_init.cpp $(DEBUGOBJECT)

clientDebug.o: client.cpp
	g++ client.cpp $(DEBUGOBJECT)

destroyDebug.o: bank_destroy.cpp
	g++ bank_destroy.cpp $(DEBUGOBJECT)

serverDebug.o: server.cpp
	g++ server.cpp $(THREADPOOL) $(DEBUGOBJECT)

server_clientDebug.o: server_client.cpp
	g++ server_client.cpp $(DEBUGOBJECT)

