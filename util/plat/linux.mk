ifeq ($(PLAT_LINUX),1)
compile_plat=$(patsubst src/%.c,build/%.o,$(shell find -O3 src/platform/linux -name '*.c'))
CC=gcc
CFLAGS+=-DPLAT_LINUX -Isrc/platform/linux/include -fsanitize=address,undefined
LD=$(CC)
LIBS+=-lasan -lubsan
endif

linux:
	@$(info Building for platform: $(platStr_LinuxNative))
	@$(MAKE) --quiet all_linux PLAT_LINUX=1


all_linux: bin/mtocptwm
	$(info Successfully built for $(platStr_LinuxNative)!)
linux-gui: bin/gui

bin/gui: $(shell find -O3 src/gui/ -name '*.js' | grep -vE 'node_modules|out/') $(shell find -O3 src/gui/ -name '*.html' | grep -vE 'node_modules|out/') $(shell find -O3 src/gui/ -name '*.css' | grep -vE 'node_modules|out/')
	@cd src/gui; npm run make
	@cd bin; unzip ../src/gui/out/make/zip/linux/x64/gui-linux-x64-1.0.0.zip; mv gui-linux-x64/* .; rm -r gui-linux-x64