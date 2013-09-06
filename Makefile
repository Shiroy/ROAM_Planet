SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

all: $(OBJ)
	g++ -O4 -lpthread -lOgreMain -lboost_system -lOIS -o roam $^
	
%.o: %.c
	g++ -o $@ -c $<
 
