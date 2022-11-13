SRC = concepts-11.cc meta.cc meta2.cc meta4.cc
BIN = $(SRC:.cc=)

all: $(BIN)

%: %.cc
	$(CXX) -o $* $^ -std=c++17 -g -fsanitize=address -fsanitize=undefined -fsanitize=leak

clean:
	$(RM) $(BIN)
