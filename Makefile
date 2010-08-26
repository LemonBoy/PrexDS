
PATH := $(DEVKITARM)/bin:$(PATH)

GAME_TITLE		:=	PrexDS
GAME_SUBTITLE1	:=	Realtime OS
GAME_SUBTITLE2	:=	On the DS
GAME_ICON		:=	./icon.bmp

all : prex.nds

prex.nds : prex.arm7 prex.arm9
	@ndstool -c $@ -7 prex.arm7 -9 prex.arm9 -b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)"
	@dlditool r4tf.dldi prex.nds

.PHONY : prex.arm7 prex.arm9 clean

prex.arm7 :
	@$(MAKE) -C prex_7
	mv prex_7/prex_7.arm7 prex.arm7

prex.arm9 :
	./configure --target=arm-nds_arm9 --cross-prefix=arm-eabi-
	@$(MAKE) --file=prex.mk
	mv prexos prex.arm9

clean :
	rm prex.arm7 prex.arm9 prex.nds
	@$(MAKE) --file=prex.mk clean
	@$(MAKE) -C prex_7 clean

