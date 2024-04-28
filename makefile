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

clean:
	rm -f init client destroy
	rm *.o

bank_cell.o: libs/bank_cell.cpp libs/headers/bank_cell.h
	g++ libs/bank_cell.cpp $(OBJECT) 

bank.o: libs/bank.cpp libs/headers/bank.h
	g++ libs/bank.cpp $(OBJECT)

init.o: bank_init.cpp
	g++ bank_init.cpp $(OBJECT)

client.o: client.cpp
	g++ client.cpp $(OBJECT)

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