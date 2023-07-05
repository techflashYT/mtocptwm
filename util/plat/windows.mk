ifeq ($(PLAT_WIN64),1)
MING_PREFIX=x86_64
platStr_Win=$(platStr_Win64)
endif

ifeq ($(PLAT_WIN32),1)
MING_PREFIX=i686
platStr_Win=$(platStr_Win32)
endif

ifeq ($(PLAT_WIN),1)
compile_plat=$(patsubst src/%.c,build/%.o,$(shell find -O3 src/platform/win -name '*.c'))
CC=$(MING_PREFIX)-w64-mingw32-gcc
CFLAGS+=-DPLAT_WIN -Isrc/platform/win/include
LD=$(CC)
LDFLAGS+=-nostdlib -static
LIBS+=/usr/$(MING_PREFIX)-w64-mingw32/lib/crt2.o /usr/$(MING_PREFIX)-w64-mingw32/lib/crtbegin.o -lws2_32 -lmingw32 -lgcc -lmingwex -lmsvcrt -lkernel32 /usr/$(MING_PREFIX)-w64-mingw32/lib/crtend.o
endif


win: windows
windows: win64
win64:
	@$(info Building for platform: $(platStr_Win64))
	@$(MAKE) --quiet $(MAKEFLAGS) all_win PLAT_WIN=1 PLAT_WIN64=1 

win32:
	@$(info Building for platform: $(platStr_Win32))
	@$(MAKE) --quiet $(MAKEFLAGS) all_win PLAT_WIN=1 PLAT_WIN32=1 

all_win: bin/mtocptwm
	@$(info Successfully built for $(platStr_Win)!)