/*
 *     uart.h
 *
 *          Project:  UART for megaAVR, tinyAVR & AVR DA
 *          Author:   Hans-Henrik Fuxelius   
 *          Date:     Uppsala, 2023-05-08          
 */

#include <stdio.h>
#include <stdint.h>

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// DEFINE RING BUFFER SIZE; MUST BE 2, 4, 8, 16, 32, 64 or 128  
#define RBUFFER_SIZE 32  

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ENABLE USART UNITS (UNCOMMENT USARTn TO ENABLE)
#define USART0_ENABLE
// #define USART1_ENABLE
// #define USART2_ENABLE
// #define USART3_ENABLE
// #define USART4_ENABLE
// #define USART5_ENABLE

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
#define USART_BUFFER_OVERFLOW    0x6400      // ==USART_BUFOVF_bm  
#define USART_FRAME_ERROR        0x0400      // ==USART_FERR_bm             
#define USART_PARITY_ERROR       0x0200      // ==USART_PERR_bm      
#define USART_NO_DATA            0x0100      

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
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
    FILE usart_stream;				// File stream
    // PORT
    // PIN1 & 2
	volatile ringbuffer rb_rx;		// Receive 
	volatile ringbuffer rb_tx;		// Transmit
	volatile uint8_t usart_error;	// Holds error from RXDATAH        
} usart_meta;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// PORTMUX & PINOUT (DO NOT TOUCH THESE)
#ifdef USART0_ENABLE
void usart0_port_init(volatile usart_meta* meta);
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART FUNCTIONS
#ifdef USART0_ENABLE
extern FILE usart0_stream;
void usart0_init(volatile usart_meta* meta, uint16_t baud_rate);
void usart0_send_char(volatile usart_meta* meta, char c);
void usart0_send_string(volatile usart_meta* meta, char* str, uint8_t len);
uint16_t usart0_read_char(volatile usart_meta* meta);
void usart0_close(volatile usart_meta* meta);
#endif