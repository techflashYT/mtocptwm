ifeq ($(PLAT_NSWITCH),1)
include $(DEVKITPRO)/libnx/switch_rules
compile_plat=$(patsubst src/%.c,build/%.o,$(shell find -O3 src/platform/switch -name '*.c'))
ARCH    :=      -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE
CFLAGS+=-D__SWITCH__ -Isrc/platform/switch/include -ffunction-sections $(ARCH) $(INCLUDE) -I$(DEVKITPRO)/libnx/include/
LD=$(CC)
ASFLAGS+=-g $(ARCH)
LDFLAGS+=-specs=$(DEVKITPRO)/libnx/switch.specs -g $(ARCH) -static
LIBS+=-L$(PORTLIBS)/lib -L$(LIBNX)/lib -lnx
endif

switch:
	@$(info Building for platform: $(platStr_SwitchNRO))
	@$(MAKE) $(MAKEFLAGS) all_switch PLAT_NSWITCH=1 --quiet


all_switch: bin/mtocptwm.nro
	$(info Successfully built for $(platStr_SwitchNRO)!)
	$(info NOTE: There are 2 other formats availible for the Switch, `.nsp', and `.xci'.)
	$(info The former is the format used for installable apps on the Home menu.)
	$(info The later is the format used for applications on game cartridges.)
	$(info )
	$(info I can't figure out how to properly build either, so tough luck if you wanted)
	$(info to do either of them.)
bin/mtocptwm.nro: bin/mtocptwm
	@$(info $sELFNRO $< ==> $@)
	@elf2nro $< $@ $(NROFLAGS)

run_switch:
	./util/ftp_switch.sh $(IP) $(PORT)