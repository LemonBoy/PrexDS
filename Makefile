
PATH := $(DEVKITARM)/bin:$(PATH)

GAME_TITLE		:=	Prex
GAME_SUBTITLE1	:=	www.devkitpro.org
GAME_SUBTITLE2	:=	cyril.lugan.info
GAME_ICON		:=	./icon.bmp

all : prex.nds

prex.nds : prex.arm9
	@ndstool -c $@ -9 $< -b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)"

.PHONY : prex.arm9 clean

prex.arm9 :
	./configure --target=arm-nds_arm9 --cross-prefix=arm-eabi-
	@$(MAKE) --file=prex.mk
	mv prexos prex.arm9

clean :
	rm prex.nds
	@$(MAKE) --file=prex.mk clean