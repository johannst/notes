VTUNE ?= /opt/intel/oneapi/vtune/latest

main: main.c
	gcc -o $@ $^ -I$(VTUNE)/include -L$(VTUNE)/lib64 -littnotify

vtune: main
	$(VTUNE)/bin64/vtune -collect hotspots -start-paused -- ./main

clean:
	$(RM) main
	$(RM) -r r*hs
