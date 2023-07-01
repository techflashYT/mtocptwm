CC = gcc
LD = gcc
CFLAGS = -O3 -g -Isrc/include -std=gnu2x -flto
LDFLAGS = -flto

compile = $(patsubst src/%.c,build/%.o,$(shell find src -name '*.c'))

vpath %.c src
vpath %.h src/include

bin/mtocptwm: $(compile)
	@mkdir -p $(@D)

	@$(info $s    LD $(compile) ==> $@)
	@$(LD) $(LDFLAGS) $(compile) $(LIBS) -o $@

build/%.o: %.c
	@mkdir -p $(@D)

	@$(info $s    CC $< => $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf build bin