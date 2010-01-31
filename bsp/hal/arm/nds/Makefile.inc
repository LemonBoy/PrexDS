
SRCS:=		arm/arch/locore.S \
		arm/arch/cpufunc.S \
		arm/arch/cpu.c \
		arm/arch/trap.c \
		arm/arch/context.c \
		arm/nds/interrupt.c \
		arm/nds/clock.c \
		arm/nds/machdep.c

ifeq ($(DEBUG),1)
SRCS+=	arm/nds/diag/desmume.S \
		arm/nds/diag/diag.c \
		arm/nds/diag/screen.c \
		arm/nds/diag/font.c
endif
