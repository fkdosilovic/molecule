CPP = g++
FLAGS = -std=c++14 -pthread -Wall -pedantic-errors -mlzcnt -O2

all: utilities board fenparser makemove movegenerator zobrist search evaluate uci tt molecule clean

pvtable:
	$(CPP) $(FLAGS) -c pvtable.cpp -o pvtable.o

board:
	$(CPP) $(FLAGS) -c board.cpp -o board.o

fenparser:
	$(CPP) $(FLAGS) -c fenparser.cpp -o fenparser.o

makemove:
	$(CPP) $(FLAGS) -c makemove.cpp -o makemove.o

movegenerator:
	$(CPP) $(FLAGS) -c movegenerator.cpp -o movegenerator.o

utilities:
	$(CPP) $(FLAGS) -c utilities.cpp -o utilities.o

zobrist:
	$(CPP) $(FLAGS) -c zobrist.cpp -o zobrist.o

evaluate:
	$(CPP) $(FLAGS) -c evaluate.cpp -o evaluate.o

search:
	$(CPP) $(FLAGS) -c search.cpp -o search.o

uci:
	$(CPP) $(FLAGS) -c uci.cpp -o uci.o

tt:
	$(CPP) $(FLAGS) -c transpositiontable.cpp -o tt.o

molecule:
	$(CPP) $(FLAGS) ../dependency/magic-bits/slider_attacks.o board.o fenparser.o makemove.o movegenerator.o utilities.o zobrist.o search.o uci.o evaluate.o tt.o main.cpp -o molecule

clean:
	rm board.o fenparser.o makemove.o movegenerator.o utilities.o zobrist.o evaluate.o search.o uci.o tt.o
