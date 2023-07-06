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