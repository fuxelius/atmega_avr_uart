/*
 *     uart_settings.c
 *
 *          Project:  UART for megaAVR, tinyAVR & AVR DA
 *          Author:   Hans-Henrik Fuxelius   
 *          Date:     Uppsala, 2023-05-08           
 */

#include <avr/io.h>
#include "uart_settings.h"

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ASSIGN PORTMUX & PINOUT (DEFINE PORTMUX AND PINS FOR EACH USARTn)
// asm("NOP"); is a just placeholder for NO OPERATION, JUST RELACE IT!
#ifdef USART0_ENABLE
void usart0_port_init(void) {
    asm("NOP");                         // PORTMUX
    PORTA.DIR &= ~PIN1_bm;			    // Rx
    PORTA.DIR |= PIN0_bm;			    // Tx
}
#endif

#ifdef USART1_ENABLE
void usart1_port_init(void) {
    asm("NOP");                         // PORTMUX
    asm("NOP");                         // Rx
    asm("NOP");                         // Tx
}
#endif

#ifdef USART2_ENABLE
void usart2_port_init(void) {
    asm("NOP");                         // PORTMUX
    asm("NOP");                         // Rx
    asm("NOP");                         // Tx
}
#endif

#ifdef USART3_ENABLE
void usart3_port_init(void) {
    PORTMUX.USARTROUTEA = 0b01111111;   // Set PB04, PB05
    PORTB.DIR &= ~PIN5_bm;              // Rx
    PORTB.DIR |= PIN4_bm;               // Tx
}
#endif

#ifdef USART4_ENABLE
void usart4_port_init(void) {
    asm("NOP");                         // PORTMUX
    asm("NOP");                         // Rx
    asm("NOP");                         // Tx
}
#endif

#ifdef USART5_ENABLE
void usart5_port_init(void) {
    asm("NOP");                         // PORTMUX
    asm("NOP");                         // Rx
    asm("NOP");                         // Tx
}
#endif