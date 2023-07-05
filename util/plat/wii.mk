ifeq ($(PLAT_WII),1)
include $(DEVKITPPC)/wii_rules
compile_plat=$(patsubst src/%.c,build/%.o,$(shell find -O3 src/platform/wii -name '*.c'))
CFLAGS+=-DPLAT_WII $(MACHDEP) -Isrc/platform/wii/include
CFLAGS+=$(foreach dir,$(INCLUDES), -iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) \
					-I$(DEVKITPRO)/libogc/include
LD=$(CC)
LDFLAGS+=$(MACHDEP)
LIBS+=-L$(LIBOGC_LIB) $(foreach dir,$(LIBDIRS),-L$(dir)/lib) -lwiiuse -lbte -logc -lm
endif

wii:
	@$(info Building for platform: $(platStr_WiiBootDOL))
	@$(MAKE) --quiet $(MAKEFLAGS) all_wii PLAT_WII=1

all_wii: bin/boot.dol
	$(info Successfully built for $(platStr_WiiBootDOL)!)

bin/boot.dol: bin/mtocptwm
	@$(info $sELFDOL $< ==> $@)
	@elf2dol $< $@
run_wii: bin/boot.dol
	@wiiload bin/boot.dol