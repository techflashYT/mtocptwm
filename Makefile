CFLAGS = -Wall -Wextra -Wpedantic -O0 -g -Isrc/include -std=gnu2x -flto -fsanitize=address,undefined -g
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


.PHONY: linux wii all_linux all_wii windows win win64 win32 all_win32 all_win64 switch all_switch run_switch run_wii clean

# make a linux build if no platform is selected.

LIBS+=-lasan -lubsan -lSDL2 -lSDL2_ttf -lGL

build/font/%.o: build/font/%.c
	@mkdir -p $(@D)

	@$(info $s    CC $< => $@)
	@$(CC) $(CFLAGS) -c $< -o $@

build/font/FreeSans.o: %.ttf
	@$(info $s BIN2C $^ ==> $@)
	@util/bin2c $^ $@ DATA_Font_Sans
build/font/FreeMono.o: %.ttf
	@$(info $s BIN2C $^ ==> $@)
	@util/bin2c $^ $@ DATA_Font_Mono

util/bin2c: util/bin2c.c
	@mkdir -p $(@D)

	@$(info $s    CC $< => $@)
	@$(CC) $(CFLAGS) -c $< -o $@

bin/mtocptwm: $(compile_plat) $(compile) $(fonts)
	@mkdir -p $(@D)
	@$(info $s    LD $^ ==> $@)
	@$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

build/%.o: %.c $(includes)
	@mkdir -p $(@D)

	@$(info $s    CC $< => $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf build bin