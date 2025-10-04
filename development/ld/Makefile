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

handler: handler.c
	gcc -o $@ $^ -T handler-fntab.ld -T handler-datatab.ld -Wall -Wextra -g
	readelf -W -S $@
	./$@
.PHONY: handler

clean:
	$(RM) mem *.o handler
