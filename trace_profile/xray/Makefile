run: main
	XRAY_OPTIONS="patch_premain=true,xray_mode=xray-basic,verbosity=1" ./$< 1 2 3 4 5 > /dev/null
	llvm-xray account --instr_map=$< --sort=sum $$(ls -t1 xray-log.main.* | head -n1)
	llvm-xray convert --instr_map=$< --symbolize --output-format=trace_event $$(ls -t1 xray-log.main.* | head -n1)  | tee xray-trace.txt

main: main.c
	clang -o $@ $< -g -fxray-instrument -fxray-instruction-threshold=1

clean:
	$(RM) main xray-log.main.* xray-trace.txt
