CFLAGS = -std=c++11 -Wall -O3
CFLAGS_DBG = -std=c++11 -g -Wall -DDEBUG=1

main:
	@make Shape.o
	@make Graph.o
	@echo "Merging object files ..."
	@g++ $(CFLAGS) main.cpp Shape.o Graph.o -o DPT_balance_color

%.o: %.cpp
	@echo "Compiling $< ..."
	@g++ $(CFLAGS) -c -o $@ $<

run:
	@./DPT_balance_color || { echo "Program does not exist! Type \"make\" first!"; exit 1; }

debug:
	@echo "Compiling Shape.o ..."
	@g++ $(CFLAGS_DBG) -c -o Shape.o Shape.cpp
	@echo "Compiling Graph.o ..."
	@g++ $(CFLAGS_DBG) -c -o Graph.o Graph.cpp
	@echo "Merging object files ..."
	@g++ $(CFLAGS_DBG) main.cpp Shape.o Graph.o -o ColorBalancing


ctags:
	@echo "Tagging ..."
	@ctags -R ./

clean:
	@rm -f *.o DPT_balance_color
