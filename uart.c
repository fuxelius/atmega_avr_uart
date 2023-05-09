/*
 *     uart.c
 *
 *          Project:  UART for ATmega4808
 *          Author:   Hans-Henrik Fuxelius   
 *          Date:     2023-05-02           
 */

#include <util/atomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "uart_settings.h"
#include "uart.h"

#define UART_RX_ERROR_MASK (USART_BUFOVF_bm | USART_FERR_bm | USART_PERR_bm) // [Datasheet ss. 295]

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
#ifdef USART0_ENABLE
volatile ringbuffer rb_rx0;		// Receive 
volatile ringbuffer rb_tx0;		// Transmit
volatile uint8_t usart0_error;	// Keeps error from RXDATAH
#endif

#ifdef USART1_ENABLE
volatile ringbuffer rb_rx1;		// Receive 
volatile ringbuffer rb_tx1;		// Transmit
volatile uint8_t usart1_error;	// Keeps error from RXDATAH
#endif

#ifdef USART2_ENABLE
volatile ringbuffer rb_rx2;		// Receive 
volatile ringbuffer rb_tx2;		// Transmit
volatile uint8_t usart2_error;	// Keeps error from RXDATAH
#endif

#ifdef USART3_ENABLE
volatile ringbuffer rb_rx3;		// Receive 
volatile ringbuffer rb_tx3;		// Transmit
volatile uint8_t usart3_error;	// Keeps error from RXDATAH
#endif

#ifdef USART4_ENABLE
volatile ringbuffer rb_rx4;		// Receive 
volatile ringbuffer rb_tx4;		// Transmit
volatile uint8_t usart4_error;	// Keeps error from RXDATAH
#endif

#ifdef USART5_ENABLE
volatile ringbuffer rb_rx5;		// Receive 
volatile ringbuffer rb_tx5;		// Transmit
volatile uint8_t usart5_error;	// Keeps error from RXDATAH
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART0 FUNCTIONS
#ifdef USART0_ENABLE
void usart0_send_char(char c) {
	rbuffer_insert(c, &rb_tx0);
	USART0.CTRLA |= USART_DREIE_bm;
}

int usart0_print_char(char c, FILE *stream) { 
    usart0_send_char(c);							
    return 0; 
}

FILE USART0_stream = FDEV_SETUP_STREAM(usart0_print_char, NULL, _FDEV_SETUP_WRITE);

void usart0_init(uint16_t baud_rate) {
	rbuffer_init(&rb_rx0);							// Init RX0 buffer
	rbuffer_init(&rb_tx0);							// Init TX0 buffer
	
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
	if (!rbuffer_empty(&rb_rx0)) {
		return (((usart0_error & UART_RX_ERROR_MASK) << 8) | (uint16_t)rbuffer_remove(&rb_rx0));
	}
	else {
		return (((usart0_error & UART_RX_ERROR_MASK) << 8) | UART_NO_DATA);		// Empty ringbuffer
	}
}

// Disable unit Tx and Rx before its interrupts!
void usart0_close(void) {
	while(!rbuffer_empty(&rb_tx0)); 				// Wait for Tx to finish all character in ring buffer
	while(!(USART0.STATUS & USART_DREIF_bm)); 		// Wait for Tx unit to finish the last character of ringbuffer

	_delay_ms(200); 								// Extra safety for Tx to finish!

	USART0.CTRLB &= ~USART_RXEN_bm; 				// Disable Rx unit
	USART0.CTRLB &= ~USART_TXEN_bm; 				// Disable Rx unit

	USART0.CTRLA &= ~USART_RXCIE_bm;				// Disable Rx interrupt
	USART0.CTRLA &= ~USART_DREIE_bm;				// Disable Tx interrupt
}
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART1 FUNCTIONS
#ifdef USART1_ENABLE
void usart1_send_char(char c) {
	rbuffer_insert(c, &rb_tx1);
	USART1.CTRLA |= USART_DREIE_bm;
}

int usart1_print_char(char c, FILE *stream) { 
    usart1_send_char(c);							
    return 0; 
}

FILE USART1_stream = FDEV_SETUP_STREAM(usart1_print_char, NULL, _FDEV_SETUP_WRITE);

void usart1_init(uint16_t baud_rate) {
	rbuffer_init(&rb_rx1);							// Init RX1 buffer
	rbuffer_init(&rb_tx1);							// Init TX1 buffer
	
	usart1_port_init();								// Defined in uart_settings.h

    USART1.BAUD = baud_rate; 						// Set BAUD rate
	USART1.CTRLB |= USART_RXEN_bm | USART_TXEN_bm; 	// Enable Rx & Enable Tx 
	USART1.CTRLA |= USART_RXCIE_bm ; 				// Enable Rx interrupt 
}

void usart1_send_string(char* str, uint8_t len) {
	for (size_t i=0; i<len; i++) {
		usart1_send_char(str[i]);
	}
}

uint16_t usart1_read_char(void) {
	if (!rbuffer_empty(&rb_rx1)) {
		return (((usart1_error & UART_RX_ERROR_MASK) << 8) | (uint16_t)rbuffer_remove(&rb_rx1));
	}
	else {
		return (((usart1_error & UART_RX_ERROR_MASK) << 8) | UART_NO_DATA);		// Empty ringbuffer
	}
}

// Disable unit Tx and Rx before its interrupts!
void usart1_close(void) {
	while(!rbuffer_empty(&rb_tx1)); 				// Wait for Tx to finish all character in ring buffer
	while(!(USART1.STATUS & USART_DREIF_bm)); 		// Wait for Tx unit to finish the last character of ringbuffer

	_delay_ms(200); 								// Extra safety for Tx to finish!

	USART1.CTRLB &= ~USART_RXEN_bm; 				// Disable Rx unit
	USART1.CTRLB &= ~USART_TXEN_bm; 				// Disable Rx unit

	USART1.CTRLA &= ~USART_RXCIE_bm;				// Disable Rx interrupt
	USART1.CTRLA &= ~USART_DREIE_bm;				// Disable Tx interrupt
}
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART2 FUNCTIONS
#ifdef USART2_ENABLE
void usart2_send_char(char c) {
	rbuffer_insert(c, &rb_tx2);
	USART2.CTRLA |= USART_DREIE_bm;
}

int usart2_print_char(char c, FILE *stream) { 
    usart2_send_char(c);							
    return 0; 
}

FILE USART2_stream = FDEV_SETUP_STREAM(usart2_print_char, NULL, _FDEV_SETUP_WRITE);

void usart2_init(uint16_t baud_rate) {
	rbuffer_init(&rb_rx2);							// Init RX2 buffer
	rbuffer_init(&rb_tx2);							// Init TX2 buffer
	
	usart2_port_init();								// Defined in uart_settings.h

    USART2.BAUD = baud_rate; 						// Set BAUD rate
	USART2.CTRLB |= USART_RXEN_bm | USART_TXEN_bm; 	// Enable Rx & Enable Tx 
	USART2.CTRLA |= USART_RXCIE_bm ; 				// Enable Rx interrupt 
}

void usart2_send_string(char* str, uint8_t len) {
	for (size_t i=0; i<len; i++) {
		usart2_send_char(str[i]);
	}
}

uint16_t usart2_read_char(void) {
	if (!rbuffer_empty(&rb_rx2)) {
		return (((usart2_error & UART_RX_ERROR_MASK) << 8) | (uint16_t)rbuffer_remove(&rb_rx2));
	}
	else {
		return (((usart2_error & UART_RX_ERROR_MASK) << 8) | UART_NO_DATA);		// Empty ringbuffer
	}
}

// Disable unit Tx and Rx before its interrupts!
void usart2_close(void) {
	while(!rbuffer_empty(&rb_tx2)); 				// Wait for Tx to finish all character in ring buffer
	while(!(USART2.STATUS & USART_DREIF_bm)); 		// Wait for Tx unit to finish the last character of ringbuffer

	_delay_ms(200); 								// Extra safety for Tx to finish!

	USART2.CTRLB &= ~USART_RXEN_bm; 				// Disable Rx unit
	USART2.CTRLB &= ~USART_TXEN_bm; 				// Disable Rx unit

	USART2.CTRLA &= ~USART_RXCIE_bm;				// Disable Rx interrupt
	USART2.CTRLA &= ~USART_DREIE_bm;				// Disable Tx interrupt
}
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART3 FUNCTIONS
#ifdef USART3_ENABLE
void usart3_send_char(char c) {
	rbuffer_insert(c, &rb_tx3);
	USART3.CTRLA |= USART_DREIE_bm;
}

int usart3_print_char(char c, FILE *stream) { 
    usart3_send_char(c);							
    return 0; 
}

FILE USART3_stream = FDEV_SETUP_STREAM(usart3_print_char, NULL, _FDEV_SETUP_WRITE);

void usart3_init(uint16_t baud_rate) {
	rbuffer_init(&rb_rx3);							// Init RX3 buffer
	rbuffer_init(&rb_tx3);							// Init TX3 buffer
	
	usart3_port_init();								// Defined in uart_settings.h

    USART3.BAUD = baud_rate; 						// Set BAUD rate
	USART3.CTRLB |= USART_RXEN_bm | USART_TXEN_bm; 	// Enable Rx & Enable Tx 
	USART3.CTRLA |= USART_RXCIE_bm ; 				// Enable Rx interrupt 
}

void usart3_send_string(char* str, uint8_t len) {
	for (size_t i=0; i<len; i++) {
		usart3_send_char(str[i]);
	}
}

uint16_t usart3_read_char(void) {
	if (!rbuffer_empty(&rb_rx3)) {
		return (((usart3_error & UART_RX_ERROR_MASK) << 8) | (uint16_t)rbuffer_remove(&rb_rx3));
	}
	else {
		return (((usart3_error & UART_RX_ERROR_MASK) << 8) | UART_NO_DATA);		// Empty ringbuffer
	}
}

// Disable unit Tx and Rx before its interrupts!
void usart3_close(void) {
	while(!rbuffer_empty(&rb_tx3)); 				// Wait for Tx to finish all character in ring buffer
	while(!(USART3.STATUS & USART_DREIF_bm)); 		// Wait for Tx unit to finish the last character of ringbuffer

	_delay_ms(200); 								// Extra safety for Tx to finish!

	USART3.CTRLB &= ~USART_RXEN_bm; 				// Disable Rx unit
	USART3.CTRLB &= ~USART_TXEN_bm; 				// Disable Rx unit

	USART3.CTRLA &= ~USART_RXCIE_bm;				// Disable Rx interrupt
	USART3.CTRLA &= ~USART_DREIE_bm;				// Disable Tx interrupt
}
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART4 FUNCTIONS
#ifdef USART4_ENABLE
void usart4_send_char(char c) {
	rbuffer_insert(c, &rb_tx4);
	USART4.CTRLA |= USART_DREIE_bm;
}

int usart4_print_char(char c, FILE *stream) { 
    usart4_send_char(c);							
    return 0; 
}

FILE USART4_stream = FDEV_SETUP_STREAM(usart4_print_char, NULL, _FDEV_SETUP_WRITE);

void usart4_init(uint16_t baud_rate) {
	rbuffer_init(&rb_rx4);							// Init RX4 buffer
	rbuffer_init(&rb_tx4);							// Init TX4 buffer
	
	usart4_port_init();								// Defined in uart_settings.h

    USART4.BAUD = baud_rate; 						// Set BAUD rate
	USART4.CTRLB |= USART_RXEN_bm | USART_TXEN_bm; 	// Enable Rx & Enable Tx 
	USART4.CTRLA |= USART_RXCIE_bm ; 				// Enable Rx interrupt 
}

void usart4_send_string(char* str, uint8_t len) {
	for (size_t i=0; i<len; i++) {
		usart4_send_char(str[i]);
	}
}

uint16_t usart4_read_char(void) {
	if (!rbuffer_empty(&rb_rx4)) {
		return (((usart4_error & UART_RX_ERROR_MASK) << 8) | (uint16_t)rbuffer_remove(&rb_rx4));
	}
	else {
		return (((usart4_error & UART_RX_ERROR_MASK) << 8) | UART_NO_DATA);		// Empty ringbuffer
	}
}

// Disable unit Tx and Rx before its interrupts!
void usart4_close(void) {
	while(!rbuffer_empty(&rb_tx4)); 				// Wait for Tx to finish all character in ring buffer
	while(!(USART4.STATUS & USART_DREIF_bm)); 		// Wait for Tx unit to finish the last character of ringbuffer

	_delay_ms(200); 								// Extra safety for Tx to finish!

	USART4.CTRLB &= ~USART_RXEN_bm; 				// Disable Rx unit
	USART4.CTRLB &= ~USART_TXEN_bm; 				// Disable Rx unit

	USART4.CTRLA &= ~USART_RXCIE_bm;				// Disable Rx interrupt
	USART4.CTRLA &= ~USART_DREIE_bm;				// Disable Tx interrupt
}
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART5 FUNCTIONS
#ifdef USART5_ENABLE
void usart5_send_char(char c) {
	rbuffer_insert(c, &rb_tx5);
	USART5.CTRLA |= USART_DREIE_bm;
}

int usart5_print_char(char c, FILE *stream) { 
    usart5_send_char(c);							
    return 0; 
}

FILE USART5_stream = FDEV_SETUP_STREAM(usart5_print_char, NULL, _FDEV_SETUP_WRITE);

void usart5_init(uint16_t baud_rate) {
	rbuffer_init(&rb_rx5);							// Init RX5 buffer
	rbuffer_init(&rb_tx5);							// Init TX5 buffer
	
	usart5_port_init();								// Defined in uart_settings.h

    USART5.BAUD = baud_rate; 						// Set BAUD rate
	USART5.CTRLB |= USART_RXEN_bm | USART_TXEN_bm; 	// Enable Rx & Enable Tx 
	USART5.CTRLA |= USART_RXCIE_bm ; 				// Enable Rx interrupt 
}

void usart5_send_string(char* str, uint8_t len) {
	for (size_t i=0; i<len; i++) {
		usart5_send_char(str[i]);
	}
}

uint16_t usart5_read_char(void) {
	if (!rbuffer_empty(&rb_rx5)) {
		return (((usart5_error & UART_RX_ERROR_MASK) << 8) | (uint16_t)rbuffer_remove(&rb_rx5));
	}
	else {
		return (((usart5_error & UART_RX_ERROR_MASK) << 8) | UART_NO_DATA);		// Empty ringbuffer
	}
}

// Disable unit Tx and Rx before its interrupts!
void usart5_close(void) {
	while(!rbuffer_empty(&rb_tx5)); 				// Wait for Tx to finish all character in ring buffer
	while(!(USART5.STATUS & USART_DREIF_bm)); 		// Wait for Tx unit to finish the last character of ringbuffer

	_delay_ms(200); 								// Extra safety for Tx to finish!

	USART5.CTRLB &= ~USART_RXEN_bm; 				// Disable Rx unit
	USART5.CTRLB &= ~USART_TXEN_bm; 				// Disable Rx unit

	USART5.CTRLA &= ~USART_RXCIE_bm;				// Disable Rx interrupt
	USART5.CTRLA &= ~USART_DREIE_bm;				// Disable Tx interrupt
}
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART0 ISR FUNCTIONS
#ifdef USART0_ENABLE
ISR(USART0_RXC_vect) {
    char data = USART0.RXDATAL;
	rbuffer_insert(data, &rb_rx0);
	usart0_error = USART0.RXDATAH;
}

ISR(USART0_DRE_vect) {
	if(!rbuffer_empty(&rb_tx0)) {
		USART0.TXDATAL = rbuffer_remove(&rb_tx0);
	}
	else {
		USART0.CTRLA &= ~USART_DREIE_bm;
	}
}
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART1 ISR FUNCTIONS
#ifdef USART1_ENABLE
ISR(USART1_RXC_vect) {
    char data = USART1.RXDATAL;
	rbuffer_insert(data, &rb_rx1);
	usart1_error = USART1.RXDATAH;
}

ISR(USART1_DRE_vect) {
	if(!rbuffer_empty(&rb_tx1)) {
		USART1.TXDATAL = rbuffer_remove(&rb_tx1);
	}
	else {
		USART1.CTRLA &= ~USART_DREIE_bm;
	}
}
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART2 ISR FUNCTIONS
#ifdef USART2_ENABLE
ISR(USART2_RXC_vect) {
    char data = USART2.RXDATAL;
	rbuffer_insert(data, &rb_rx2);
	usart2_error = USART2.RXDATAH;
}

ISR(USART2_DRE_vect) {
	if(!rbuffer_empty(&rb_tx2)) {
		USART2.TXDATAL = rbuffer_remove(&rb_tx2);
	}
	else {
		USART2.CTRLA &= ~USART_DREIE_bm;
	}
}
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART3 ISR FUNCTIONS
#ifdef USART3_ENABLE
ISR(USART3_RXC_vect) {
    char data = USART3.RXDATAL;
	rbuffer_insert(data, &rb_rx3);
	usart3_error = USART0.RXDATAH;
}

ISR(USART3_DRE_vect) {
	if(!rbuffer_empty(&rb_tx3)) {
		USART3.TXDATAL = rbuffer_remove(&rb_tx3);
	}
	else {
		USART3.CTRLA &= ~USART_DREIE_bm;
	}
}
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART4 ISR FUNCTIONS
#ifdef USART4_ENABLE
ISR(USART4_RXC_vect) {
    char data = USART4.RXDATAL;
	rbuffer_insert(data, &rb_rx4);
	usart4_error = USART4.RXDATAH;
}

ISR(USART4_DRE_vect) {
	if(!rbuffer_empty(&rb_tx4)) {
		USART4.TXDATAL = rbuffer_remove(&rb_tx4);
	}
	else {
		USART4.CTRLA &= ~USART_DREIE_bm;
	}
}
#endif

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART5 ISR FUNCTIONS
#ifdef USART5_ENABLE
ISR(USART5_RXC_vect) {
    char data = USART5.RXDATAL;
	rbuffer_insert(data, &rb_rx5);
	usart5_error = USART5.RXDATAH;
}

ISR(USART5_DRE_vect) {
	if(!rbuffer_empty(&rb_tx5)) {
		USART5.TXDATAL = rbuffer_remove(&rb_tx5);
	}
	else {
		USART5.CTRLA &= ~USART_DREIE_bm;
	}
}
#endif