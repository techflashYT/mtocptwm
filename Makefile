ORIGCFLAGS = -Wall -Wextra -Wpedantic -O0 -g -Isrc/include -std=gnu2x -flto -g
CFLAGS=$(ORIGCFLAGS)
LDFLAGS = -flto -g 

includes = $(shell find -O3 src/include)
vpath %.c src
vpath %.ttf misc/freefont-20120503
vpath %.h src/include

compile=$(patsubst src/%.c,build/%.o,$(shell find -O3 src -name '*.c' | grep -v 'platform/'))
fontList=FreeSans.ttf FreeMono.ttf
fonts=$(patsubst %.ttf,build/font/%.o,$(fontList))
default: linux
include util/strings.mk
include util/plat/*.mk


.PHONY: linux all_linux windows win win64 win32 all_win32 all_win64 switch all_switch run_switch clean

# make a linux build if no platform is selected.

LIBS+=-lSDL2 -lSDL2_ttf
ifeq ($(strip $(PLAT_NSWITCH)),)
LIBS+=-lGL
endif

build/font/%.o: build/font/%.c
	@mkdir -p $(@D)

	@$(info $s    CC $< => $@)
	@$(CC) $(CFLAGS) -c $< -o $@

build/font/FreeSans.c: misc/freefont-20120503/FreeSans.ttf util/bin2c
	@mkdir -p $(@D)

	@$(info $s BIN2C $< ==> $@)
	@util/bin2c $< $@ DATA_FontSans
build/font/FreeMono.c: misc/freefont-20120503/FreeMono.ttf util/bin2c
	@mkdir -p $(@D)

	@$(info $s BIN2C $< ==> $@)
	@util/bin2c $< $@ DATA_FontMono

util/bin2c: util/bin2c.c
	@mkdir -p $(@D)

	@$(info $sHOSTCC $< => $@)
# uses system gcc
	@gcc $(ORIGCFLAGS) $< -o $@

bin/mtocptwm: $(compile_plat) $(compile) $(fonts)
	@mkdir -p $(@D)
	@$(info $s    LD $^ ==> $@)
	@$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

build/%.o: %.c $(includes)
	@mkdir -p $(@D)

	@$(info $s    CC $< => $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf build bin util/bin2c