
SRCS:=		arm/arch/locore.S \
		arm/arch/cpufunc.S \
		arm/arch/cpu.c \
		arm/arch/trap.c \
		arm/arch/context.c \
		arm/nds_arm9/interrupt.c \
		arm/nds_arm9/clock.c \
		arm/nds_arm9/machdep.c

ifeq ($(DEBUG),1)
SRCS+=	arm/nds_arm9/diag.c
ifeq ($(CONFIG_DIAG_DESMUME),y)
SRCS+=	arm/nds_arm9/diag_desmume.S
endif
endif
