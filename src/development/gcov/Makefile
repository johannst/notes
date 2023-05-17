# Copyright (C) 2023 johannst

CXXFLAGS = -fprofile-arcs -ftest-coverage
# or the alias
#CXXFLAGS = --coverage

cov-gcc: clean
	g++ $(CXXFLAGS) -c -o cov.o cov.cc
	g++ $(CXXFLAGS) -o $@ cov.o
	./$@
	gcov --demangled-names cov.cc
	cat cov.cc.gcov
.PHONY: cov-gcc

cov-clang: clean
	clang++ $(CXXFLAGS) -c -o cov.o cov.cc
	clang++ $(CXXFLAGS) -o $@ cov.o
	./$@
	llvm-cov gcov --demangled-names cov.cc
	cat cov.cc.gcov
.PHONY: cov-clang

clean:
	$(RM) *.gcov *.gcno *.gcda *.o cov-*
