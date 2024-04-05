DBG ?= n
CMAKE_DBG-y = -DCMAKE_FIND_DEBUG_MODE=1

all:
	@# CMAKE_MODULE_PATH for include() and find_package module flows.
	@# CMAKE_PREFIX_PATH for find_package config flow.
	@# Bar_DIR package specific for find_package config flow.
	cmake . -B build -DCMAKE_MODULE_PATH="cmake;foo;bar" -DCMAKE_PREFIX_PATH="foo" -DBar_DIR=bar $(CMAKE_DBG-$(DBG))

debug:
	$(MAKE) all DBG=y

clean:
	$(RM) -r build
