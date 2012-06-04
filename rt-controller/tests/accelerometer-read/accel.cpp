#define F_CPU (8L*1000L*1000L)                  /* oscillator-frequency in Hz */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
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

void uart_putc(unsigned char c)
{
  // wait until UDR ready
  while(!(UCSRA & (1 << UDRE)));
  UDR = c;    // send character
}

void uart_puts(const char *s)
{
  //  loop until *s != NULL
  while (*s)
  {
    uart_putc(*s);
    s++;
  }
}


void uart_int(uint8_t b)
{
  char    buf[4]="000";
  uint8_t pos=2;
  while(b>0)
  {
    buf[pos]='0'+(b%10);
    b/=10;
    --pos;
  }
  uart_puts(buf);
}

void initUSART(void)
{
  // set baud rate
  UBRRH = (uint8_t)(UART_BAUD_CALC(UART_BAUD_RATE,F_CPU)>>8);
  UBRRL = (uint8_t)UART_BAUD_CALC(UART_BAUD_RATE,F_CPU);
  // Enable receiver and transmitter; enable RX interrupt
  UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
  //asynchronous 8N1
  UCSRC = (1 << URSEL) | (3 << UCSZ0);
}

void initADC(void)
{
  // setup port as analog inputs (i.e. tri-state)
  DDRC  &= ~( _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0) ); // input
  PORTC &= ~( _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0) ); // disable pull-ups
  // initialize
  ADMUX  = _BV(ADLAR);
  // ADMUX / MUX3..MUX0
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS1) | _BV(ADPS0);
  //ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS0);
  // skip first results
  while( !(ADCSRA & _BV(ADIF) ) ) {};
  ADCSRA |= _BV(ADIF);// | _BV(ADIE);
}

uint8_t adc_read(const uint8_t mux)
{
  // select channel
  ADMUX  &= ~( _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0) );
  ADMUX  |= mux;
  // start conversion
  ADCSRA |= _BV(ADSC);
  // wait for the result
  while( !(ADCSRA & _BV(ADIF) ) ) {};
  ADCSRA |= _BV(ADIF);
  // return the result
  return ADCH;
}

/*
// ADC interrupt
ISR(ADC_vect)
{
  // TODO
}
*/

// USART RX interrupt
ISR(USART_RXC_vect)
{
  unsigned char c;
  c = UDR;
  uart_putc(c);
}

int main(void)
{
  initUSART();
  initADC();
  sei();  // enable interrupts
  uart_puts("up and running...\n");

  while(true)
  {
    const uint8_t ox =adc_read( 0 );
    const uint8_t oy =adc_read( _BV(MUX0) );
    const uint8_t oz =adc_read( _BV(MUX1) );
    const uint8_t vin=adc_read( _BV(MUX1) | _BV(MUX0) );

    uart_puts("OX=");    uart_int(ox);  uart_putc(' ');
    uart_puts("OY=");    uart_int(oy);  uart_putc(' ');
    uart_puts("OZ=");    uart_int(oz);  uart_putc(' ');
    uart_puts("Vin/3="); uart_int(vin); uart_putc('\n');

    delay(1);
  }

  DDRB |= (1<<PB2) | (1<<PB1) | (1<<PB0);
  DDRD |= (1<<PD7);

  while(1)
  {
    char c;
    for(c='a'; c<='z'; ++c)
      uart_putc(c);
    uart_putc('\n');
    delay(10);
  }

  return 0;
}
