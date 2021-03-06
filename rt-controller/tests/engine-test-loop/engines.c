#define F_CPU (8L*1000L*1000L)                  /* oscillator-frequency in Hz */

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>   // NOTE: obsolete include commented-out
#include <inttypes.h>
//#include <avr/iom16.h>    // NOTE: we use atmega8 instaed of atmega16
#include <util/delay.h>

//#define UART_BAUD_RATE 9600
#define UART_BAUD_RATE 38400
#define UART_BAUD_CALC(UART_BAUD_RATE,F_CPU) ((F_CPU)/((UART_BAUD_RATE)*16l)-1)

void delay(const uint8_t ds)
{
  uint8_t i;
  for(i=0; i<ds; ++i)
    _delay_ms(100);
}

void usart_putc(unsigned char c)
{
  // wait until UDR ready
  while(!(UCSRA & (1 << UDRE)));
  UDR = c;    // send character
}

void uart_puts (char *s)
{
  //  loop until *s != NULL
  while (*s)
  {
    usart_putc(*s);
    s++;
  }
}

void init(void)
{
  // set baud rate
  UBRRH = (uint8_t)(UART_BAUD_CALC(UART_BAUD_RATE,F_CPU)>>8);
  UBRRL = (uint8_t)UART_BAUD_CALC(UART_BAUD_RATE,F_CPU);

  // Enable receiver and transmitter; enable RX interrupt
  UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);

  //asynchronous 8N1
  UCSRC = (1 << URSEL) | (3 << UCSZ0);
}

// USART RX interrupt
ISR(USART_RXC_vect)
{
  unsigned char c;
  c = UDR;
  usart_putc(c);
}


int main(void)
{
  init(); // init USART
  sei();  // enable interrupts

  DDRB |= (1<<PB3) | (1<<PB2) | (1<<PB1) | (1<<PB0);
  DDRD |= (1<<PD7);

  while(1)
  {
    // off
    usart_putc('0');
    PORTB=0x00;
    PORTD=0x00;
    delay(10);
    // main 1
    usart_putc('1');
    PORTB=0x02;
    delay(10);
    // main 2
    usart_putc('2');
    PORTB=0x04;
    delay(10);
    // rear left
    usart_putc('3');
    PORTB=0x01|0x08;
    PORTD=0x00;
    delay(10);
    // rear right
    usart_putc('4');
    PORTB=0x00|0x08;
    PORTD=0x80;
    delay(10);
    // eol
    usart_putc('\n');
  }

  while(1)
  {
    char c;
    for(c='a'; c<='z'; ++c)
      usart_putc(c);
    usart_putc('\n');
  }

  return 0;
}
