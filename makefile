INCLUDE=/usr/include/ncurses
LIB=ncurses

all: c main.o snake.o 
	g++ -g3 main.o snake.o -l$(LIB) -o a
main.o: main.cpp
	g++ -c -g3 main.cpp -I$(INCLUDE)
snake.o: snake.cpp
	g++ -c -g3 snake.cpp -I$(INCLUDE)
c:
	rm -f *.o snake
