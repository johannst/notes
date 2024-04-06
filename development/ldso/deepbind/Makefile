run: build
	LD_PRELOAD=./libprel.so ./main

debug: build
	#LD_DEBUG_OUTPUT=ldso
	LD_DEBUG=scopes,symbols,bindings LD_PRELOAD=./libprel.so ./main

build:
	gcc -g -o libprel.so lib.c -DNAME=\"prel\" -fPIC -shared
	gcc -g -o libdeep.so lib.c -DNAME=\"deep\" -fPIC -shared
	gcc -g -o libnodp.so lib.c -DNAME=\"nodp\" -fPIC -shared
	gcc -g -o liblink.so lib.c -DNAME=\"link\" -fPIC -shared
	gcc -g -o main main.c ./liblink.so -ldl

fmt:
	clang-format -i *.c

clean:
	$(RM) *.so main
