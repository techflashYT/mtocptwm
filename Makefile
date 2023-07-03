CFLAGS = -Wall -Wextra -Wpedantic -O0 -g -Isrc/include -std=gnu2x -flto
LDFLAGS = -flto

includes = $(shell find -O3 src/include)
vpath %.c src
vpath %.h src/include

compile = $(patsubst src/%.c,build/%.o,$(shell find -O3 src -name '*.c' | grep -v 'platform/'))
default: linux
include util/plat/linux.mk
include util/plat/wii.mk
include util/plat/windows.mk

include util/strings.mk

.PHONY: linux wii all_linux all_wii  clean

# make a linux build if no platform is selected.


bin/mtocptwm: $(compile) $(compile_plat)
	@mkdir -p $(@D)
	@$(info $s    LD $^ ==> $@)
	@$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

bin/boot.dol: bin/mtocptwm
	@$(info $sELFDOL $< ==> $@)
	@elf2dol $< $@

build/%.o: %.c $(includes)
	@mkdir -p $(@D)

	@$(info $s    CC $< => $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf build bin