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