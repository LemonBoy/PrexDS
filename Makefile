SUBDIR:=	bsp sys usr
SRCDIR:=	$(CURDIR)
export SRCDIR

GAME_TITLE		:=	PrexDS
GAME_SUBTITLE1	:=	www.devkitpro.org
GAME_SUBTITLE2	:=	cyril.lugan.info
GAME_ICON		:=	$(DEVKITPRO)/libnds/icon.bmp

include $(SRCDIR)/mk/image.mk