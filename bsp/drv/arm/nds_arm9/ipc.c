/*
 * ipc.c - Interprocessor comunication driver.
 * 
 * The Lemon Man (C) 2010
 */

#include <driver.h>
#include <dki.h>

#include "ipc.h"

struct ipc_softc {
	device_t	dev;
	irq_t		irq;
};

static int ipc_init  (struct driver *);
static int ipc_devctl (device_t, u_long, void *);

static struct devops ipc_devops = {
	/* open */	no_open,
	/* close */	no_close,
	/* read */	no_read,
	/* write */	no_write,
	/* ioctl */	no_ioctl,
	/* devctl */ipc_devctl,
};

struct driver ipc_driver = {
	/* name */	"ipc",
	/* devops */&ipc_devops,
	/* devsz */	sizeof(struct ipc_softc),
	/* flags */	0,
	/* probe */	NULL,
	/* init */	ipc_init,
	/* shutdown */	NULL,
};

#define IPC_CHAN_SYSTEM 0x0
#define IPC_CHAN_INPUT  0x1

#define IPC_DEVCTL_RTC   0xE
#define IPC_DEVCTL_INPUT 0xF

/* Time (in msec) to wait before retrying a fetch on the queue */
#define IPC_RETRY_DELAY 200

/*
 * Utility function to manage the queue.
 */

int
ipc_pop (uint32_t *msg)
{
	if (IPC_FIFO_CNT & IPC_RECV_FIFO_EMPTY)
		return 0;
	*msg = IPC_FIFO_RECV;
	return 1;
}

int
ipc_push (uint32_t msg)
{
	if (IPC_FIFO_CNT & IPC_SEND_FIFO_FULL)
		return 0;
	IPC_FIFO_SEND = msg;
	return 1;
}

/*
 * Message queue handler attached to the IRQ.
 */

static uint32_t msgQueue[4][0x20];
static int queueTop[4] = {0, 0, 0, 0};

int
queue_push(uint32_t msg, int channel)
{
	if (queueTop[channel] >= 0x20)
		return 0;
	msgQueue[channel][queueTop[channel]++] = msg;
	return 1;
}

int
queue_pop(uint32_t *msg, int channel)
{
	if (!queueTop[channel])
		return 0;
	*msg = msgQueue[channel][--queueTop[channel]];
	return 1;
}

#define IPC_CHANNEL_MASK 0xC0000000
#define IPC_DATA_MASK    0x3FFFFFFF

#define MAKE_IPC_MSG(channel, data) ((channel & 3) << 30 | (data & IPC_DATA_MASK))

static int
ipc_recv_isr(void *arg)
{
	uint32_t ipcMessage;
		
	/* Process every element of the queue. */
	while (ipc_pop(&ipcMessage))
	{	
		if (!queue_push(ipcMessage & IPC_DATA_MASK, (ipcMessage & IPC_CHANNEL_MASK) >> 30))
			panic("IPC queue full!\n");
	}
	
	return 0;
}

/*
 * devctl handler, aka the interface with the world.
 */

static int
ipc_devctl (device_t dev, u_long cmd, void *arg)
{
	uint32_t request, response, channel;
	int msglen = 1;
	
	switch (cmd)
	{
		case IPC_DEVCTL_INPUT:
			channel = IPC_CHAN_INPUT;
			request = MAKE_IPC_MSG(channel, cmd);
			break;
		case IPC_DEVCTL_RTC:
			channel = IPC_CHAN_SYSTEM;
			msglen  = 2;
			request = MAKE_IPC_MSG(channel, cmd);
			break;
		default:
			printf("Unrecognized DEVCTL %x\n", cmd);
			return -1;
	}

	while (!ipc_push(request));

	do {
		while (!queue_pop(&response, channel));
		((uint32_t *)arg)[msglen-1] = response & IPC_DATA_MASK;
		msglen--;
	} while (msglen);

	return 0;
}

int
ipc_init(struct driver *self)
{
	struct ipc_softc *sc;
	device_t dev;
	
	dev = device_create(self, "ipc", D_CHR);
	sc = device_private(dev);
	
	printf("Started ipc driver\n");
	
	IPC_FIFO_CNT  = 0;
	IPC_FIFO_CNT |= IPC_FIFO_ENABLE;
	IPC_FIFO_CNT |= IPC_SEND_FIFO_CLEAR;
	IPC_FIFO_CNT |= IPC_RECV_FIFO_NOT_EMPTY_IRQ;
	
	printf("Registers set\n");	
	
	sc->irq = irq_attach(ICU_IPC_RECV_FIFO_NOT_EMPTY, IPL_HIGH, 0, ipc_recv_isr, IST_NONE, sc);
			     
	printf("Attacched IRQ handler\n");

	return 0;
}

