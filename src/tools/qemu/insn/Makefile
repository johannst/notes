test: test.s
	as -o test.o test.s
	ld -o test test.o testc.o

trace: test
	qemu-x86_64 -singlestep -d nochain,cpu ./test 2>&1 | awk '/RIP/ { print $$1; }'

clean:
	$(RM) test test-bin test.o
