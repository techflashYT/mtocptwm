ifeq ($(PLAT_WIN64),1)
compile_plat=$(patsubst src/%.c,build/%.o,$(shell find -O3 src/platform/win -name '*.c'))
platStr_Win=$(platStr_Win64)
CC=x86_64-w64-mingw32-gcc
CFLAGS+=-DPLAT_WIN -DPLAT_WIN64 -Isrc/platform/win/include
LD=$(CC)
LDFLAGS+=-nostdlib -static
LIBS+=/usr/x86_64-w64-mingw32/lib/crt2.o /usr/x86_64-w64-mingw32/lib/crtbegin.o -lws2_32 -lmingw32 -lgcc -lmingwex -lmsvcrt -lkernel32 /usr/x86_64-w64-mingw32/lib/crtend.o
endif

ifeq ($(PLAT_WIN32),1)
compile_plat=$(patsubst src/%.c,build/%.o,$(shell find -O3 src/platform/win -name '*.c'))
platStr_Win=$(platStr_Win32)
CC=i686-w64-mingw32-gcc
CFLAGS+=-DPLAT_WIN -DPLAT_WIN32 -Isrc/platform/win/include
LD=$(CC)
LDFLAGS+=-nostdlib -static
LIBS+=/usr/i686-w64-mingw32/lib/crt2.o /usr/i686-w64-mingw32/lib/crtbegin.o -lws2_32 -lmingw32 -lgcc -lmingwex -lmsvcrt -lkernel32 /usr/i686-w64-mingw32/lib/crtend.o
endif


win: windows
windows: win64
win64:
	@$(info Building for platform: $(platStr_Win64))
	@$(MAKE) $(MAKEFLAGS) all_win PLAT_WIN64=1 --quiet

win32:
	@$(info Building for platform: $(platStr_Win32))
	@$(MAKE) $(MAKEFLAGS) all_win PLAT_WIN32=1 --quiet

all_win: bin/mtocptwm
	@$(info Successfully built for $(platStr_Win)!)