ORIGCFLAGS = -Wall -Wextra -Wpedantic -O0 -g -Isrc/include -std=gnu2x -flto 
CFLAGS=$(ORIGCFLAGS)
LDFLAGS = -flto -g 

includes = $(shell find -O3 src/include)
vpath %.c src
vpath %.h src/include

compile=$(patsubst src/%.c,build/%.o,$(shell find -O3 src -name '*.c' | grep -v 'platform/'))
default: linux
include util/strings.mk
include util/plat/*.mk


.PHONY: linux linux-gui all_linux windows win win64 win32 all_win32 all_win64 switch all_switch run_switch clean

# make a linux build if no platform is selected.

LIBS+=-lSDL2 -lSDL2_ttf
ifeq ($(strip $(PLAT_NSWITCH)),)
LIBS+=-lGL
endif

util/bin2c: util/bin2c.c
	@mkdir -p $(@D)

	@$(info $sHOSTCC $< => $@)
# uses system gcc
	@gcc $(ORIGCFLAGS) $< -o $@

bin/mtocptwm: $(compile_plat) $(compile)
	@mkdir -p $(@D)
	@$(info $s    LD $^ ==> $@)
	@$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

build/%.o: %.c $(includes)
	@mkdir -p $(@D)

	@$(info $s    CC $< => $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf build bin util/bin2c