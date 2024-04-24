.PHONY: clean

CXXFLAGS_E = -Wall -Werror -pthread -o

CXXFLAGS_D = -g -Wall -Werror -pthread -fsanitize=address -o

CXXFLAGS_O = -c -pthread 

BANK_E = bank.o bank_cell.o

main: $(BANK_E) init.o client.o destroy.o
	g++ bank_init.o $(BANK_E) $(CXXFLAGS_E) init
	g++ client.o $(BANK_E) $(CXXFLAGS_E) client
	g++ bank_destroy.o $(BANK_E) $(CXXFLAGS_E) destroy
	rm *.o

debug: $(BANK_E) init.o client.o destroy.o
	g++ bank_init.o $(BANK_E) $(CXXFLAGS_D) init
	g++ client.o $(BANK_E) $(CXXFLAGS_D) client
	g++ bank_destroy.o $(BANK_E) $(CXXFLAGS_D) destroy
	rm *.o

clean:
	rm -f init client destroy

bank_cell.o: libs/bank_cell.cpp libs/headers/bank_cell.h
	g++ libs/bank_cell.cpp $(CXXFLAGS_O) 

bank.o: libs/bank.cpp libs/headers/bank.h
	g++ libs/bank.cpp $(CXXFLAGS_O)

init.o: bank_init.cpp
	g++ bank_init.cpp $(CXXFLAGS_O)

client.o: client.cpp
	g++ client.cpp $(CXXFLAGS_O)

destroy.o: bank_destroy.cpp
	g++ bank_destroy.cpp $(CXXFLAGS_O)
