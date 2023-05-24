/*
 *     uart.h
 *
 *          Project:  UART for megaAVR, tinyAVR & AVR DA
 *          Author:   Hans-Henrik Fuxelius   
 *          Date:     Uppsala, 2023-05-08          
 */

#include <stdint.h>
#include "uart_settings.h"

#define USART_BUFFER_OVERFLOW    0x6400      // ==USART_BUFOVF_bm  
#define USART_FRAME_ERROR        0x0400      // ==USART_FERR_bm             
#define USART_PARITY_ERROR       0x0200      // ==USART_PERR_bm      
#define USART_NO_DATA            0x0100      

#define BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// RINGBUFFER STRUCT
typedef struct { 
    volatile char     buffer[RBUFFER_SIZE];     
    volatile uint8_t  in;                           
    volatile uint8_t  out;                          
    volatile uint8_t  count;         
} ringbuffer;

// USART META STRUCT
typedef struct { 
	USART_t* usart;					// USART device ptr
    FILE uart_stream;				// File stream
    // PORT
    // PIN1 & 2
	volatile ringbuffer rb_rx;		// Receive 
	volatile ringbuffer rb_tx;		// Transmit
	volatile uint8_t usart_error;	// Holds error from RXDATAH        
} usart_meta;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART FUNCTIONS
#ifdef USART0_ENABLE
extern FILE USART0_stream;
void usart0_init( volatile usart_meta* meta, uint16_t baud_rate);
// void usart0_send_char(char c);
void usart0_send_char(volatile usart_meta* meta, char c);
void usart0_send_string(char* str, uint8_t len);
uint16_t usart0_read_char(volatile usart_meta* meta);
void usart0_close(volatile usart_meta* meta);
#endif