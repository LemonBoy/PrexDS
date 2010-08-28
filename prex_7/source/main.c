/*
 * PrexDS arm7 support.
 * The Lemon Man (C) 2010
 */

#include <nds.h>
#include "ipc.h"

#define IPC_CHANNEL_MASK 0xC0000000
#define IPC_DATA_MASK    0x3FFFFFFF

#define MAKE_IPC_MSG(channel, data) ((channel & 3) << 30 | (data & IPC_DATA_MASK))

static int exitFlag = 0;

int queuePop (u32 *msg)
{
	if (IPC_FIFO_CNT & IPC_RECV_FIFO_EMPTY)
		return 0;
	*msg = IPC_FIFO_RECV;
	return 1;
}

int queuePush (u32 msg)
{
	if (IPC_FIFO_CNT & IPC_SEND_FIFO_FULL)
		return 0;
	IPC_FIFO_SEND = msg;
	return 1;
}

int main() {
	u32 fifoMsg;

	irqInit();
	fifoInit();

	/* Start the RTC tracking IRQ */
	initClockIRQ();

	SetYtrigger(80);

	/* fifoInit doesnt do this D: */
    IPC_FIFO_CNT = IPC_FIFO_ENABLE;
    
	while (!exitFlag) 
    {
		ipc_area->aux_keys = 0xbeef;/*((~(*(volatile u16 *)0x04000136))&0xFF);*/
	}
	return 0;
}
