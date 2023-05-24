/*
 *     uart.c
 *
 *          Project:  UART for megaAVR, tinyAVR & AVR DA
 *          Author:   Hans-Henrik Fuxelius   
 *          Date:     Uppsala, 2023-05-08           
 */

#include <util/atomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "uart_settings.h"
#include "uart.h"

#define USART_RX_ERROR_MASK (USART_BUFOVF_bm | USART_FERR_bm | USART_PERR_bm) // [Datasheet ss. 295]

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// RINGBUFFER STRUCT
typedef struct { 
    volatile char     buffer[RBUFFER_SIZE];     
    volatile uint8_t  in;                           
    volatile uint8_t  out;                          
    volatile uint8_t  count;         
} ringbuffer;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// RINGBUFFER FUNCTIONS
void rbuffer_init(volatile ringbuffer* rb) {
	rb->in = 0;
	rb->out = 0;
	rb->count = 0;
}

uint8_t rbuffer_count(volatile ringbuffer* rb) {
	return rb->count;
}

bool rbuffer_full(volatile ringbuffer* rb) {
	return (rb->count == (uint8_t)RBUFFER_SIZE);
}

bool rbuffer_empty(volatile ringbuffer* rb) {
	return (rb->count == 0);
}

void rbuffer_insert(char data, volatile ringbuffer* rb) {   
	*(rb->buffer + rb->in) = data;
	rb->in = (rb->in + 1) & ((uint8_t)RBUFFER_SIZE - 1);
	rb->count++;
}

char rbuffer_remove(volatile ringbuffer* rb) {
	char data = *(rb->buffer + rb->out);
	rb->out = (rb->out + 1) & ((uint8_t)RBUFFER_SIZE - 1);
	rb->count--;
	return data;
}





// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// RINGBUFFERS & VARIABLES


typedef struct { 
	USART_t* uart;					// USART device ptr
	FILE uart_stream;				// File stream
	volatile ringbuffer rb_rx;		// Receive 
	volatile ringbuffer rb_tx;		// Transmit
	volatile uint8_t usart_error;	// Holds error from RXDATAH        
} usart_meta;


#ifdef USART0_ENABLE
// volatile ringbuffer rb_rx0;		// Receive 
// volatile ringbuffer rb_tx0;		// Transmit
// volatile uint8_t usart0_error;	// Holds error from RXDATAH

volatile usart_meta usart0_meta;
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART0 FUNCTIONS
#ifdef USART0_ENABLE

void usart0_send_char(char c) {
	while(rbuffer_full(&usart0_meta.rb_tx));
	rbuffer_insert(c, &usart0_meta.rb_tx);
	USART0.CTRLA |= USART_DREIE_bm;					// Enable Tx interrupt 
}

int usart0_print_char(char c, FILE *stream) { 
    usart0_send_char(c);							
    return 0; 
}

FILE USART0_stream = FDEV_SETUP_STREAM(usart0_print_char, NULL, _FDEV_SETUP_WRITE);

void usart0_init(uint16_t baud_rate) {
	rbuffer_init(&usart0_meta.rb_rx);							// Init RX0 buffer
	rbuffer_init(&usart0_meta.rb_tx);							// Init TX0 buffer
	usart0_port_init();								// Defined in uart_settings.h
    USART0.BAUD = baud_rate; 						// Set BAUD rate
	USART0.CTRLB |= USART_RXEN_bm | USART_TXEN_bm; 	// Enable Rx & Enable Tx 
	USART0.CTRLA |= USART_RXCIE_bm ; 				// Enable Rx interrupt 
}

void usart0_send_string(char* str, uint8_t len) {
	for (size_t i=0; i<len; i++) {
		usart0_send_char(str[i]);
	}
}

uint16_t usart0_read_char(void) {
	if (!rbuffer_empty(&usart0_meta.rb_rx)) {
		return (((usart0_meta.usart_error & USART_RX_ERROR_MASK) << 8) | (uint16_t)rbuffer_remove(&usart0_meta.rb_rx));
	}
	else {
		return (((usart0_meta.usart_error & USART_RX_ERROR_MASK) << 8) | USART_NO_DATA);		// Empty ringbuffer
	}
}

// Disable unit Tx and Rx before its interrupts!
void usart0_close(void) {
	while(!rbuffer_empty(&usart0_meta.rb_tx)); 				// Wait for Tx to finish all character in ring buffer
	while(!(USART0.STATUS & USART_DREIF_bm)); 		// Wait for Tx unit to finish the last character of ringbuffer

	// _delay_ms(200); 								// Extra safety for Tx to finish!

	USART0.CTRLB &= ~USART_RXEN_bm; 				// Disable Rx unit
	USART0.CTRLB &= ~USART_TXEN_bm; 				// Disable Rx unit

	USART0.CTRLA &= ~USART_RXCIE_bm;				// Disable Rx interrupt
	USART0.CTRLA &= ~USART_DREIE_bm;				// Disable Tx interrupt
}

#endif



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART0 ISR FUNCTIONS
#ifdef USART0_ENABLE
ISR(USART0_RXC_vect) {
    char data = USART0.RXDATAL;
	rbuffer_insert(data, &usart0_meta.rb_rx);
	usart0_meta.usart_error = USART0.RXDATAH;
}

ISR(USART0_DRE_vect) {
	if(!rbuffer_empty(&usart0_meta.rb_tx)) {
		USART0.TXDATAL = rbuffer_remove(&usart0_meta.rb_tx);
	}
	else {
		USART0.CTRLA &= ~USART_DREIE_bm;
	}
}
#endif