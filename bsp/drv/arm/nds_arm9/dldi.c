/*
 * DS DLDI driver
 * 2010 (C) The Lemon Man
 * Many thanks to Wintermute and Cyril for the help :D
 */

#include <driver.h>

#include "dldi.h"

static int dldi_init (struct driver *);
static int dldi_read (device_t, char *, size_t *, int);
static int dldi_write (device_t, char *, size_t *, int);

struct dldi_softc {
	device_t	dev;
	irq_t		irq;
	DLDI_INTERFACE *dldi_if;
	DISC_INTERFACE *disc_if;
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

void hexdump (void *x, int y)
{
	uint8_t *t = (uint8_t*)x;
	while (y--)
	{
		if (!(y%0x10))
			printf("\n");
		printf("%02x ", *t++);
	}
}

static int
dldi_read (device_t dev, char *buf, size_t *nbyte, int blkno)
{
	struct dldi_softc *sc = device_private(dev);

	if (!sc->disc_if->readSectors(blkno, (*nbyte) / 512, buf))
		return 1;
	
	return 0;
}

static int
dldi_write (device_t dev, char *buf, size_t *nbyte, int blkno)
{
	struct dldi_softc *sc = device_private(dev);

	if (!sc->disc_if->writeSectors(blkno, (*nbyte) / 512, buf))
		return 1;
	
	return 0;
}

static int 
dldi_init (struct driver *self)
{
	struct dldi_softc *sc;
	device_t dev;
	
	dev = device_create(self, "dldi", D_REM);
	sc = device_private(dev);

	sc->dldi_if = ((DLDI_INTERFACE *)0x02002000);
	sc->disc_if = &sc->dldi_if->ioInterface;
	
	printf("DLDI driver\n");
	
	if (sc->dldi_if->magicNumber != DLDI_MAGIC)
	{
		printf("Wrong magic\n");
		return 1;
	}
	
	printf("base %x %x\n", sc->dldi_if->dldiStart, sc->dldi_if->dldiEnd);
	printf("Driver : %s\n", sc->dldi_if->friendlyName);
	
	if (!sc->disc_if->startup())
	{
		printf("Cannot initialize the driver\n");
		return 1;
	}
	
	printf("DLDI driver set-up finished\n");
	
	return 0;	
}
