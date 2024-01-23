prof: cg_example
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no -- ./cg_example

prof-cache: cg_example
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no --cache-sim=yes -- ./cg_example

show:
	callgrind_annotate

cg_example: cg_example.cc
	clang++ -o cg_example -Wall -Wextra -O2 -g -fno-inline cg_example.cc -lpthread

clean:
	$(RM) callgrind.* cg_example
