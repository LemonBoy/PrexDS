
PATH := $(DEVKITARM)/bin:$(PATH)

GAME_TITLE		:=	PrexDS
GAME_SUBTITLE1	:=	cyril.lugan.info
GAME_SUBTITLE2	:=	www.devkitpro.org
GAME_ICON		:=	$(DEVKITPRO)/libnds/icon.bmp

all : prex.nds

prex.nds : prex.arm9
	ndstool -c $@ -9 prex.arm9 -b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)"

.PHONY : prex.arm9 clean

prex.arm9 :
	@$(MAKE) --file=prex.mk prex.arm9

clean :
	@$(MAKE) --file=prex.mk clean