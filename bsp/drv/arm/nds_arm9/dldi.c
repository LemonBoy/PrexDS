/*
 * DS DLDI driver
 * 2010 (C) The Lemon Man
 */

#include <driver.h>

#include "dldi.h"

static int dldi_init (struct driver *);
static int dldi_read (device_t, char *, size_t *, int);

struct dldi_softc {
	device_t	dev;
	irq_t		irq;
};

static struct devops dldi_devops = {
	/* open */	no_open,
	/* close */	no_close,
	/* read */	dldi_read,
	/* write */	no_write,
	/* ioctl */	no_ioctl,
	/* devctl */no_devctl,
};

struct driver dldi_driver = {
	/* name */	"dldi",
	/* devops */&dldi_devops,
	/* devsz */	sizeof(struct dldi_softc),
	/* flags */	0,
	/* probe */	NULL,
	/* init */	dldi_init,
	/* shutdown */	NULL,
};

static DLDI_INTERFACE *dldi_if = (DLDI_INTERFACE *)0x02000100;
static DISC_INTERFACE *disc_if = &((DLDI_INTERFACE *)0x02000100)->ioInterface;

int dldi_valid ()
{
	return (dldi_if->magicNumber == DLDI_MAGIC);
}

static int
dldi_read (device_t dev, char *buf, size_t *nbyte, int blkno)
{
	char *secbuf = kmem_map(buf, *nbyte);
	
	if (!dldi_if->ioInterface.readSectors(blkno, (*nbyte) / 512, secbuf))
	{
		*nbyte = 0;
		printf("DLDI cant read\n");
		return 1;
	}
	
	return 0;
}

static int 
dldi_init (struct driver *self)
{
	struct dldi_softc *sc;
	device_t dev;
	
	if (!dldi_valid())
	{
		printf("DLDI driver not valid\n");
		return 1;
	}
		
	dev = device_create(self, "dldi", D_REM);
	sc = device_private(dev);
	
	printf("Driver : %s\n", dldi_if->friendlyName);
	printf("%x %x\n", disc_if->startup, dldi_if->ioInterface.startup);
	
	if (!disc_if->startup())
	{
		printf("Cannot initialize the driver\n");
		return 1;
	}
	
	printf("Startup sent\n");
	
	return 0;	
}
