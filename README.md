# Interrupt driven and buffered UART for megaAVR 0-series
This UART library was initially developed for a bare metal atmega4808 project. It is general enough to be adapted for UART1, UART2 and UART3. It will for sure work for the entire **megaAVR 0-series** which it was primarily developed for.

**megaAVR 0-series**: which consists of ATmega808, ATmega809, ATmega1608, ATmega1609, ATmega3208, ATmega3209, ATmega4808 and ATmega4809.

It should also work on following series:

**tinyAVR 0-series**: ATtiny202, ATtiny402, ATtiny204, ATtiny404, ATtiny804, ATtiny1604, ATtiny406, ATtiny806, ATtiny1606, ATtiny807 and ATtiny1607

**tinyAVR 1-series**: ATtiny212, ATtiny412, ATtiny214, ATtiny414, ATtiny814, ATtiny1614, ATtiny416, ATtiny816, ATtiny1616, ATtiny3216, ATtiny417, ATtiny817, ATtiny1617 and ATtiny3217 


**AVR DA devices**: 



It is based on a technical brief article from Microchip [TB3216](https://ww1.microchip.com/downloads/en/Appnotes/TB3216-Getting-Started-with-USART-DS90003216.pdf)
and I have tried to follow it as close as possible in functionality and naming of functions. 

## Setting up parameters

### Buffer size
The buffer size is symmetric and equal for both transmit (Tx) and receive (Rx). It has a typical size of 32 or 64, but can be set at any size in its range from 2, 4, 8, 16, 32, 64, 128 or 256. 

	define RBUFFER_SIZE 64

### PORTMUX - Port Multiplexer
The Port Multiplexer (PORTMUX) can either enable or disable the functionality of the pins, or change between default and alternative pin positions. Available options are described in detail in the PORTMUX register map and depend on the actual pin and its properties. Select which ever is appropriate for your selection of USARTn and pin selection. [Datasheet ss. 139]

	PORTMUX.USARTROUTEA = 0b01111111; // Set PB04, PB05

Above example is port multiplexing for pin PB04 and PB05 for USART3 as given in the USART library given for Arduino Nano Every. [Datasheet ss. 143]


## UART functions
    void usart0_init(uint16_t baud_rate);
    void usart0_send_char(char c);
    void usart0_send_string(char* str, uint8_t len);
    uint16_t usart0_read_char(void);
    void usart0_close(void);



## How to use the library
Here is a short overview of how to use the library. 

    // (1) - Init UART
    usart0_init((uint16_t)BAUD_RATE(9600));

    // (2) - Enable global interrupts
    sei(); 

    // (3) - Send string to UART
    usart0_send_string("\r\n\r\nPEACE BRO!\r\n\r\n", 18);

    // (4) - Use printf to write to UART
    printf("Hello world!\r\n");

    for(uint8_t i=0; i<5; i++) {
        // (5) - Use formatted printf to write to UART
        printf("\r\nCounter value is: 0x%02X ", j++);
        _delay_ms(500);

        // (6) - Get UART input by polling ringbuffer
        while(!((c = usart0_read_char()) & UART_NO_DATA)) {

            if (c & UART_PARITY_ERROR) {
                printf("UART PARITY ERROR: ");
            }
            if (c & UART_FRAME_ERROR) {
                printf("UART FRAME ERROR: ");
            }
            if (c & UART_BUFFER_OVERFLOW) {
                printf("UART BUFFER OVERFLOW ERROR: ");
            }

            // (7) - Send single character to UART
            usart0_send_char((char)c);
        }
    }

    // (8) - Check that everything is printed before closing UART
    printf("\r\n\r\n<-<->->");

    // (9) - Close UART0
    usart0_close();    

    // (10) - Clear global interrupts
    cli();

### (1) - Init UART

### (2) - Enable global interrupts

### (3) - Send string to UART

### (4) - Use printf to write to UART

### (5) - Use formatted printf to write to UART

### (6) - Get UART input by polling ringbuffer

### (7) - Send single character to UART

### (8) - Check that everything is printed before closing UART

### (9) - Close UART0

### (10) - Clear global interrupts