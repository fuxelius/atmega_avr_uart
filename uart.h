/*
 *     uart.h
 *
 *          Project:  UART for ATmega4808
 *          Author:   Hans-Henrik Fuxelius   
 *          Date:     2023-05-02           
 */

#include <stdint.h>
#include "uart_settings.h"

#define UART_BUFFER_OVERFLOW    0x6400      // ==USART_BUFOVF_bm  
#define UART_FRAME_ERROR        0x0400      // ==USART_FERR_bm             
#define UART_PARITY_ERROR       0x0200      // ==USART_PERR_bm      
#define UART_NO_DATA            0x0100      

#define BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART FUNCTIONS
#ifdef USART0_ENABLE
extern FILE USART0_stream;
void usart0_init(uint16_t baud_rate);
void usart0_send_char(char c);
void usart0_send_string(char* str, uint8_t len);
uint16_t usart0_read_char(void);
void usart0_close(void);
#endif

#ifdef USART1_ENABLE
extern FILE USART1_stream;
void usart1_init(uint16_t baud_rate);
void usart1_send_char(char c);
void usart1_send_string(char* str, uint8_t len);
uint16_t usart1_read_char(void);
void usart1_close(void);
#endif

#ifdef USART2_ENABLE
extern FILE USART2_stream;
void usart2_init(uint16_t baud_rate);
void usart2_send_char(char c);
void usart2_send_string(char* str, uint8_t len);
uint16_t usart2_read_char(void);
void usart2_close(void);
#endif

#ifdef USART3_ENABLE
extern FILE USART3_stream;
void usart3_init(uint16_t baud_rate);
void usart3_send_char(char c);
void usart3_send_string(char* str, uint8_t len);
uint16_t usart3_read_char(void);
void usart3_close(void);
#endif

#ifdef USART4_ENABLE
extern FILE USART4_stream;
void usart4_init(uint16_t baud_rate);
void usart4_send_char(char c);
void usart4_send_string(char* str, uint8_t len);
uint16_t usart4_read_char(void);
void usart4_close(void);
#endif

#ifdef USART5_ENABLE
extern FILE USART5_stream;
void usart5_init(uint16_t baud_rate);
void usart5_send_char(char c);
void usart5_send_string(char* str, uint8_t len);
uint16_t usart5_read_char(void);
void usart5_close(void);
#endif