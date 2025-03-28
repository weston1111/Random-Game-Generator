CXX = g++
CXXFLAGS = -Wall -std=c++20
SRC = random_game_generator.cpp

EXEC = Random_game_generator

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRC)

clean:
	rm -f $(EXEC)