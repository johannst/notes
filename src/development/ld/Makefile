show-mem:
show-nomem:
show-%: link-%
	readelf -W -S -l $^
	nm $^
	objdump -d $^

link-%: link-%.ld data.o
	ld -o $@ -T $^

%.o: %.S
	gcc -c $^

clean:
	$(RM) mem *.o
