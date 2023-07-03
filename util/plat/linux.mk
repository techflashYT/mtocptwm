ifeq ($(PLAT_LINUX),1)
compile_plat=$(patsubst src/%.c,build/%.o,$(shell find -O3 src/platform/linux -name '*.c'))
CC=gcc
CFLAGS+=-DPLAT_LINUX -Isrc/platform/linux/include
LD=$(CC)
endif

linux:
	@$(info Building for platform: $(platStr_LinuxNative))
	@$(MAKE) $(MAKEFLAGS) all_linux PLAT_LINUX=1 --quiet


all_linux: bin/mtocptwm
	$(info Successfully built for $(platStr_LinuxNative)!)