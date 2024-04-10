all:
	mkdir -p foo bar
	gcc -g -o foo/lib.so lib.c -shared -fPIC -DNAME=\"foo\"
	gcc -g -o bar/lib.so lib.c -shared -fPIC -DNAME=\"bar\"
	gcc -g -o main main.c -ldl
	./main foo/lib.so bar/lib.so
	LD_LIBRARY_PATH=foo:bar ./main lib.so lib.so
	LD_LIBRARY_PATH=bar:foo ./main lib.so lib.so
	LD_LIBRARY_PATH=foo ./main lib.so foo/lib.so
	LD_LIBRARY_PATH=foo ./main lib.so bar/lib.so

clean:
	$(RM) -r foo bar main
