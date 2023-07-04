# space
E :=
s= $E $E

# Progressively more ugly ways to get the architecture
___arch___=$(shell uname -m)
ifeq ($(strip ___arch___),)
___arch___=$(shell arch)
endif
ifeq ($(strip ___arch___),)
___arch___=$(shell gcc -dumpmachine | tr '-' ' ' | awk '{ print $$1 }')
endif
ifeq ($(strip ___arch___),)
___arch___=Unknown
endif
platStr_LinuxNative=Linux $(___arch___) ELF
platStr_WiiBootDOL=Nintendo Wii boot.dol
platStr_Win64=Windows x86_64 `.exe' file
platStr_Win32=Windows i386 `.exe' file
platStr_SwitchNRO=Nintendo Switch `.nro' (homebrew) file