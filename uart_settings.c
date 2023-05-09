/*
 *     uart_settings.c
 *
 *          Project:  UART for ATmega4808
 *          Author:   Hans-Henrik Fuxelius   
 *          Date:     2023-05-02           
 */

#include <avr/io.h>
#include "uart_settings.h"

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ASSIGN PORTMUX & PINOUT
#ifdef USART0_ENABLE
void usart0_port_init(void) {
    asm("NOP");                         // PORTMUX
    PORTA.DIR &= ~PIN1_bm;			    // input
    PORTA.DIR |= PIN0_bm;			    // output
}
#endif

#ifdef USART1_ENABLE
void usart1_port_init(void) {
    asm("NOP");                         // PORTMUX
    asm("NOP");                         // input
    asm("NOP");                         // output
}
#endif

#ifdef USART2_ENABLE
void usart2_port_init(void) {
    asm("NOP");                         // PORTMUX
    asm("NOP");                         // input
    asm("NOP");                         // output
}
#endif

#ifdef USART3_ENABLE
void usart3_port_init(void) {
    PORTMUX.USARTROUTEA = 0b01111111;   // Set PB04, PB05
    PORTB.DIR &= ~PIN5_bm;              // input
    PORTB.DIR |= PIN4_bm;               // output
}
#endif

#ifdef USART4_ENABLE
void usart4_port_init(void) {
    asm("NOP");                         // PORTMUX
    asm("NOP");                         // input
    asm("NOP");                         // output
}
#endif

#ifdef USART5_ENABLE
void usart5_port_init(void) {
    asm("NOP");                         // PORTMUX
    asm("NOP");                         // input
    asm("NOP");                         // output
}
#endif