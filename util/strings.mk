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
platStr_WinNative=Windows x86_64 `.exe' file