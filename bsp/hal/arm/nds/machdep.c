/*-
 * Copyright (c) 2008-2009, Kohsuke Ohtani
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * machdep.c - machine-dependent routines for ARM Integrator-CP
 */

#include <machine/syspage.h>
#include <sys/power.h>
#include <sys/bootinfo.h>
#include <kernel.h>
#include <page.h>
#include <mmu.h>
#include <cpu.h>
#include <cpufunc.h>
#include <locore.h>

#include "platform.h"

/*
 * Idle
 */
void
machine_idle(void)
{

	cpu_idle();
}

/*
 * Reset system.
 */
static void
machine_reset(void)
{

	for (;;) ;
	/* NOTREACHED */
}

/*
 * Set system power
 */
void
machine_powerdown(int state)
{

	splhigh();

	DPRINTF(("Power down machine\n"));

	switch (state) {
	case PWR_OFF:
		for (;;)
			cpu_idle();
		/* NOTREACHED */
		break;
	case PWR_REBOOT:
		machine_reset();
		/* NOTREACHED */
		break;
	}
}

/*
 * Return pointer to the boot information.
 */
void
machine_bootinfo(struct bootinfo **bip)
{

	*bip = (struct bootinfo *)BOOTINFO;
}

void
machine_abort(void)
{

	for (;;)
		cpu_idle();
}

/*
 * Machine-dependent startup code
 */
void
machine_startup(void)
{

	/*
	 * Initialize CPU and basic hardware.
	 */
	cpu_init();
	/*cache_init();*/

}
