#ifndef DLDI_H
#define DLDI_H

#include <types.h>

#define DLDI_MAGIC_STRING_LEN 		8
#define DLDI_FRIENDLY_NAME_LEN 		48

#define DLDI_MAGIC					0xBF8DA5ED

#define FEATURE_MEDIUM_CANREAD		0x00000001
#define FEATURE_MEDIUM_CANWRITE		0x00000002
#define FEATURE_SLOT_GBA			0x00000010
#define FEATURE_SLOT_NDS			0x00000020

typedef uint32_t sec_t;

typedef int (* FN_MEDIUM_STARTUP)(void) ;
typedef int (* FN_MEDIUM_ISINSERTED)(void) ;
typedef int (* FN_MEDIUM_READSECTORS)(sec_t sector, sec_t numSectors, void* buffer) ;
typedef int (* FN_MEDIUM_WRITESECTORS)(sec_t sector, sec_t numSectors, const void* buffer) ;
typedef int (* FN_MEDIUM_CLEARSTATUS)(void) ;
typedef int (* FN_MEDIUM_SHUTDOWN)(void) ;

typedef struct DISC_INTERFACE {
	unsigned long			ioType ;
	unsigned long			features ;
	FN_MEDIUM_STARTUP		startup ;
	FN_MEDIUM_ISINSERTED	isInserted ;
	FN_MEDIUM_READSECTORS	readSectors ;
	FN_MEDIUM_WRITESECTORS	writeSectors ;
	FN_MEDIUM_CLEARSTATUS	clearStatus ;
	FN_MEDIUM_SHUTDOWN		shutdown ;
} DISC_INTERFACE;

typedef struct DLDI_INTERFACE {
	uint32_t 	magicNumber;
	char		magicString [DLDI_MAGIC_STRING_LEN];
	uint8_t		versionNumber;
	uint8_t		driverSize;			/* log-2 of driver size in bytes */
	uint8_t		fixSectionsFlags;
	uint8_t		allocatedSize;		/* log-2 of the allocated space in bytes */

	char	friendlyName [DLDI_FRIENDLY_NAME_LEN];
	
	/* Pointers to sections that need address fixing */
	void*	dldiStart;
	void*	dldiEnd;
	void*	interworkStart;
	void*	interworkEnd;
	void*	gotStart;
	void*	gotEnd;
	void*	bssStart;
	void*	bssEnd;
	
	/* Original I/O interface data */
	DISC_INTERFACE ioInterface;
} DLDI_INTERFACE;

#endif
