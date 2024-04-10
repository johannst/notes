SRC = concepts-11.cc concepts-20.cc meta.cc meta2.cc meta4.cc tmpl-pair.cc tmpl-void_t.cc fwd.cc fwd-perfect.cc
BIN = $(SRC:.cc=)

all: $(BIN)

%: %.cc
	$(CXX) -o $* $^ -std=c++17 -g -fsanitize=address -fsanitize=undefined -fsanitize=leak

concepts-20: concepts-20.cc
	$(CXX) -fsyntax-only $^ -std=c++20

fmt:
	clang-format -i $(SRC)

clean:
	$(RM) $(BIN)
