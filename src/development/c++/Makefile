SRC = concepts-11.cc meta.cc meta2.cc meta4.cc tmpl-pair.cc
BIN = $(SRC:.cc=)

all: $(BIN)

%: %.cc
	$(CXX) -o $* $^ -std=c++17 -g -fsanitize=address -fsanitize=undefined -fsanitize=leak

fmt:
	clang-format -i $(SRC)

clean:
	$(RM) $(BIN)
