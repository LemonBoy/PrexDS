# Rules to create OS image

include $(SRCDIR)/mk/own.mk

TARGET:=	$(SRCDIR)/prexos
LOADER:=	$(SRCDIR)/bsp/boot/bootldr
DRIVER:=	$(SRCDIR)/bsp/drv/drv.ko
KERNEL:=	$(SRCDIR)/sys/prex

include $(SRCDIR)/conf/etc/tasks.mk
include $(SRCDIR)/conf/etc/files.mk
include $(SRCDIR)/mk/common.mk
-include $(SRCDIR)/bsp/boot/$(ARCH)/$(PLATFORM)/Makefile.sysgen

$(TARGET): dummy
	$(call echo-file,PACK   ,$@)
ifdef FILES
	$(AR) rcS bootdisk.a $(FILES)
	$(AR) rcS tmp.a $(KERNEL) $(DRIVER) $(TASKS) bootdisk.a
	$(RM) bootdisk.a
else
	$(AR) rcS tmp.a $(KERNEL) $(DRIVER) $(TASKS)
endif
	$(CAT) $(LOADER) tmp.a > $@
	$(RM) tmp.a
	$(call sysgen)
	@ndstool -c Prex0.9DS.nds -9 $(TARGET) -b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)"
	@echo 'Done.'
