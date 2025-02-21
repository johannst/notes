ipc: noploop
	perf stat -e cycles,instructions -- ./noploop

noploop:
	$(CC) -o noploop noploop.c

clean:
	$(RM) noploop
