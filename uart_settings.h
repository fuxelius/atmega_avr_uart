/*
 *     uart_settings.h
 *
 *          Project:  UART for megaAVR, tinyAVR & AVR DA
 *          Author:   Hans-Henrik Fuxelius   
 *          Date:     Uppsala, 2023-05-08           
 */


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
// PORTMUX & PINOUT (DO NOT TOUCH THESE)
#ifdef USART0_ENABLE
void usart0_port_init(void);
#endif

#ifdef USART1_ENABLE
void usart1_port_init(void);
#endif

#ifdef USART2_ENABLE
void usart2_port_init(void);
#endif

#ifdef USART3_ENABLE
void usart3_port_init(void);
#endif

#ifdef USART4_ENABLE
void usart4_port_init(void);
#endif

#ifdef USART5_ENABLE
void usart5_port_init(void);
#endif