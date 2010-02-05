SUBDIR:=	bsp sys usr
SRCDIR:=	$(CURDIR)
export SRCDIR

# Rules to create OS image

include $(SRCDIR)/mk/own.mk

TARGET:=	$(SRCDIR)/prex.arm9
LOADER:=	$(SRCDIR)/bsp/boot/bootldr
DRIVER:=	$(SRCDIR)/bsp/drv/drv.ko
KERNEL:=	$(SRCDIR)/sys/prex

include $(SRCDIR)/conf/etc/tasks.mk
include $(SRCDIR)/conf/etc/files.mk
include $(SRCDIR)/mk/common.mk

$(TARGET): prex.arm9 prex.arm7 dummy
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
	@echo 'Done.'
