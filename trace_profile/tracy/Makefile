B := BUILD

main: $(B)/main-static $(B)/main-dynamic $(B)/main-dynamic-win
tracy: $(B)/tracy
.PHONY: main tracy

# -- TRACY STATIC ---------------------------------------------------------------

$(B)/main-static: main.cpp | $(B)
	clang++ -DTRACY_ENABLE -I$(B)/tracy/public -o $@ $^ $(B)/tracy/public/TracyClient.cpp

# -- TRACY DYNAMIC --------------------------------------------------------------

$(B)/main-dynamic: main.cpp $(B)/foo.so $(B)/TracyClient.so | $(B)
	clang++ -DTRACY_ENABLE -I$(B)/tracy/public -DUSE_FOO -o $@ $^

$(B)/foo.so: foo.c $(B)/TracyClient.so
	clang -DTRACY_ENABLE -I$(B)/tracy/public -fPIC -shared -o $@ $^

$(B)/TracyClient.so: $(B)/tracy/public/TracyClient.cpp
	clang++ -DTRACY_ENABLE -I$(B)/tracy/public -fPIC -shared -o $@ $^

# -- TRACY DYNAMIC WINDOWS ------------------------------------------------------

$(B)/main-dynamic-win: main.cpp $(B)/foo.dll $(B)/TracyClient.dll
	@# eg run with wine
	zig c++ -target x86_64-windows -DTRACY_ENABLE -DTRACY_IMPORTS -DUSE_FOO -o $@ $^ -I $(B)/tracy/public

$(B)/foo.dll: foo.c $(B)/TracyClient.dll
	zig c++ -target x86_64-windows -DTRACY_ENABLE -DTRACY_IMPORTS -fPIC -shared -o $@ $^ -I $(B)/tracy/public

$(B)/TracyClient.dll: $(B)/tracy/public/TracyClient.cpp
	@# win libs from 'pragma comment(lib, ..)'
	zig c++ -target x86_64-windows -DTRACY_ENABLE -DTRACY_EXPORTS -fPIC -shared -o $@ $^ -lws2_32 -ldbghelp -ladvapi32 -luser32

# -- TRACY ----------------------------------------------------------------------

# Get latest tracy and build profiler.
$(B)/tracy: $(B)
	cd $(B); bash $(CURDIR)/get-tracy.sh
.PHONY: $(B)/tracy

$B:
	mkdir -p $(B)
.PHONY: $(B)

# -- CLEAN ----------------------------------------------------------------------

clean:
	$(RM) $(B)/*.so $(B)/*.dll $(B)/*.pdb $(B)/*.lib $(B)/main*

distclean:
	rm -rf $(B)
