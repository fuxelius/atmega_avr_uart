/*
 *     uart.c
 *
 *          Project:  UART for megaAVR, tinyAVR & AVR DA
 *          Author:   Hans-Henrik Fuxelius   
 *          Date:     Uppsala, 2023-05-08           
 */

#include <avr/io.h>
#include <util/atomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "uart.h"

#define USART_RX_ERROR_MASK (USART_BUFOVF_bm | USART_FERR_bm | USART_PERR_bm) // [Datasheet ss. 295]

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
// VARIABLES
#ifdef USART0_ENABLE
volatile usart_meta usart0 = {.usart = &USART0};
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART FUNCTIONS

void usart0_set(volatile usart_meta* meta, PORT_t*  port, uint8_t route, uint8_t tx_pin, uint8_t rx_pin) {
	meta->port = port;
	meta->route = route;
	meta->tx_pin = tx_pin;
	meta->rx_pin = rx_pin;
}

void usart0_port_init(volatile usart_meta* meta) {
    asm("NOP");                         // PORTMUX
    PORTA.DIR &= ~PIN1_bm;			    // Rx
    PORTA.DIR |= PIN0_bm;			    // Tx
}

void usart0_send_char(volatile usart_meta* meta, char c) {
	while(rbuffer_full(&meta->rb_tx));
	rbuffer_insert(c, &meta->rb_tx);
	meta->usart->CTRLA |= USART_DREIE_bm;			// Enable Tx interrupt 
}

void usart0_init(volatile usart_meta* meta, uint16_t baud_rate) {
	rbuffer_init(&meta->rb_rx);								// Init Rx buffer
	rbuffer_init(&meta->rb_tx);								// Init Tx buffer
	usart0_port_init(meta);									// Defined in uart_settings.h     <----------------------------- UUGH!
    meta->usart->BAUD = baud_rate; 							// Set BAUD rate
	meta->usart->CTRLB |= USART_RXEN_bm | USART_TXEN_bm; 	// Enable Rx & Enable Tx 
	meta->usart->CTRLA |= USART_RXCIE_bm; 					// Enable Rx interrupt 
}

void usart0_send_string(volatile usart_meta* meta, char* str, uint8_t len) {
	for (size_t i=0; i<len; i++) {
		usart0_send_char(meta, str[i]);
	}
}

uint16_t usart0_read_char(volatile usart_meta* meta) {
	if (!rbuffer_empty(&meta->rb_rx)) {
		return (((meta->usart_error & USART_RX_ERROR_MASK) << 8) | (uint16_t)rbuffer_remove(&meta->rb_rx));
	}
	else {
		return (((meta->usart_error & USART_RX_ERROR_MASK) << 8) | USART_NO_DATA);		// Empty ringbuffer
	}
}

void usart0_close(volatile usart_meta* meta) {
	while(!rbuffer_empty(&meta->rb_tx)); 				// Wait for Tx to finish all character in ring buffer
	while(!(meta->usart->STATUS & USART_DREIF_bm)); 	// Wait for Tx unit to finish the last character of ringbuffer

	_delay_ms(200); 									// Extra safety for Tx to finish!

	meta->usart->CTRLB &= ~USART_RXEN_bm; 				// Disable Rx unit
	meta->usart->CTRLB &= ~USART_TXEN_bm; 				// Disable Rx unit

	meta->usart->CTRLA &= ~USART_RXCIE_bm;				// Disable Rx interrupt
	meta->usart->CTRLA &= ~USART_DREIE_bm;				// Disable Tx interrupt

	// Disable PORTMUX pins
	// PORTMUX_USART0_NONE_gc
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// SPECIAL STREAM SETUP
#ifdef USART0_ENABLE
int usart0_print_char(char c, FILE *stream) { 
    usart0_send_char(&usart0, c);							
    return 0; 
}
FILE usart0_stream = FDEV_SETUP_STREAM(usart0_print_char, NULL, _FDEV_SETUP_WRITE);
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ISR HELPER FUNCTIONS
void isr_usart_rxc_vect(volatile usart_meta* meta) {
    char data = meta->usart->RXDATAL;
	rbuffer_insert(data, &meta->rb_rx);
	meta->usart_error = meta->usart->RXDATAH;
}

void isr_usart_dre_vect(volatile usart_meta* meta) {
	if(!rbuffer_empty(&meta->rb_tx)) {
		meta->usart->TXDATAL = rbuffer_remove(&meta->rb_tx);     
	}
	else {
		meta->usart->CTRLA &= ~USART_DREIE_bm;
	}
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ISR FUNCTIONS
#ifdef USART0_ENABLE
ISR(USART0_RXC_vect) {
	isr_usart_rxc_vect(&usart0);
}

ISR(USART0_DRE_vect) {
	isr_usart_dre_vect(&usart0);
}
#endif