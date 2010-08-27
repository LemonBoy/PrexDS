#ifndef IPC_H_
#define IPC_H_

/*
 * IPC Fifo Control Register
 */

#define IPC_FIFO_CNT   (*((volatile uint32_t *) 0x04000184))

#define IPC_SEND_FIFO_EMPTY          (1 <<  0)
#define IPC_SEND_FIFO_FULL           (1 <<  1)
#define IPC_SEND_FIFO_EMPTY_IRQ      (1 <<  2)
#define IPC_SEND_FIFO_CLEAR          (1 <<  3)

#define IPC_RECV_FIFO_EMPTY          (1 <<  8)
#define IPC_RECV_FIFO_FULL           (1 <<  9)
#define IPC_RECV_FIFO_NOT_EMPTY_IRQ  (1 << 10)

#define IPC_FIFO_ERROR               (1 << 14)
#define IPC_FIFO_ENABLE              (1 << 15)

/*
 * IPC Send/Receive Fifo
 */

#define IPC_FIFO_SEND  (*((volatile uint32_t *) 0x04000188))
#define IPC_FIFO_RECV  (*((volatile uint32_t *) 0x04100000))

/*
 * IPC Synchronize Register
 */

#define IPC_SYNC      (*((volatile uint32_t *) 0x04000180))

/*
 * Interrupt vectors
 */

#define ICU_IPC_RECV_FIFO_NOT_EMPTY     18
#define ICU_IPC_SEND_FIFO_EMPTY         17

/*
 * IPC Area 
 */

typedef struct 
{
	uint16_t aux_keys;
	uint16_t touch_x;
	uint16_t touch_y;
} IPC_AREA;

#define ipc_area ((IPC_AREA *)0x027FF000)

#endif /* IPC_H_ */
